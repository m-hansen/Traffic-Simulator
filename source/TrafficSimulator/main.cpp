#include "pch.h"

#define ERR_STREAM stderr

const std::uint32_t ScreenWidth = 1280;
const std::uint32_t ScreenHeight = 720;
const std::string ContentPath = "../../Content/";
const std::uint32_t VehicleCap = 30;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
TTF_Font* gCalibriFont = nullptr;
Graph* gGraph = nullptr;
std::uint32_t gFrameCount = 0;
std::uint32_t gPreviousFrameCount = 0;
std::uint32_t gFpsTime = 0;

std::list<TrafficSimulator::Vehicle> gVehicleList; // TODO: vector of shared_ptr for contiguous access
std::vector<TrafficSimulator::Wall> gWallList;
std::vector<TrafficSimulator::Spawner> gSpawnerList;
std::vector<TrafficSimulator::IntersectionManager> gIntersectionManagers;
TrafficSimulator::Vehicle* gSelectedVehicle = nullptr;

bool InitializeSDL()
{
	bool success = true;

	// Initialize SDL and create a window
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(ERR_STREAM, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// Create a window
		gWindow = SDL_CreateWindow("Traffic Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);
		if (gWindow == nullptr)
		{
			fprintf(ERR_STREAM, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			// Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == nullptr)
			{
				fprintf(ERR_STREAM, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				// Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					fprintf(ERR_STREAM, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				// Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool LoadResources()
{
	bool success = true;

	// Load textures
	success =
		TextureManager::LoadTexture(gRenderer, "car", ContentPath + "Images/car-sprite.png") &&
		TextureManager::LoadTexture(gRenderer, "wall", ContentPath + "Images/wall.png") &&
		TextureManager::LoadTexture(gRenderer, "signal", ContentPath + "Images/signal.png") &&
		TextureManager::LoadTexture(gRenderer, "node", ContentPath + "Images/node.png") &&
		TextureManager::LoadTexture(gRenderer, "nodeSM", ContentPath + "Images/node_sm.png") &&
		TextureManager::LoadTexture(gRenderer, "road", ContentPath + "Images/road.png") &&
		TextureManager::LoadTexture(gRenderer, "spawner", ContentPath + "Images/spawner.png") &&
		TextureManager::LoadTexture(gRenderer, "adjacentAgent", ContentPath + "Images/adjacent-agent-sensor.png");

	// Load the calibri font
	const std::uint8_t CalibriFontSize = 24;
	gCalibriFont = TTF_OpenFont((ContentPath + "Fonts/calibri.ttf").c_str(), CalibriFontSize);
	if (gCalibriFont == nullptr)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	success &= TextureManager::LoadFont("calibri", ContentPath + "Fonts/calibri.ttf", 18);

	return success;
}

void Shutdown()
{
	// Free loaded images
	TextureManager::ClearAll();

	// Free loaded fonts
	TTF_CloseFont(gCalibriFont);
	gCalibriFont = nullptr;

	// Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	// Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void HandleInput(const SDL_Event& e, bool& isRunning)
{
	if (e.type == SDL_QUIT)
	{
		isRunning = false;
	}

	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		if (e.button.button == SDL_BUTTON_LEFT)
		{
			if (gSelectedVehicle)
			{
				gSelectedVehicle->Deselect();
				gSelectedVehicle = nullptr;
			}

			for (auto& vehicle : gVehicleList)
			{
				// Check for vehicle selection
				SDL_Rect mouseRect{ e.button.x, e.button.y, 1, 1 };
				if (Utils::CollisionChecker(mouseRect, vehicle.GetBoundingRectangle()))
				{
					gSelectedVehicle = &vehicle;
					assert(gSelectedVehicle);
					gSelectedVehicle->Select();
					break;
				}
			}

			for (auto& vehicle : TrafficSimulator::Spawner::Vehicles())
			{
				// Check for vehicle selection
				SDL_Rect mouseRect{ e.button.x, e.button.y, 1, 1 };
				if (Utils::CollisionChecker(mouseRect, vehicle.GetBoundingRectangle()))
				{
					gSelectedVehicle = &vehicle;
					assert(gSelectedVehicle);
					gSelectedVehicle->Select();
					break;
				}
			}
		}
		if (e.button.button == SDL_BUTTON_RIGHT)
		{
			gGraph->CreateNode(Node::Count(), Vector2{e.button.x, e.button.y});

			// Temporary code for testing - creates edges between current and last nodes placed
			std::uint32_t graphNodeCount = gGraph->GetNodeCount();
			if (graphNodeCount > 1)
			{
				std::uint32_t sourceNodeId = graphNodeCount - 2;
				std::uint32_t targetNodeId = graphNodeCount - 1;

				const Node* sourceNode = gGraph->GetNodeById(sourceNodeId);
				const Node* targetNode = gGraph->GetNodeById(targetNodeId);

				gGraph->CreateEdge(*sourceNode, *targetNode);
			}
#if(_DEBUG)
			// Memory debug
			printf("======\nGraph\n======\n");
			for (const auto& node : gGraph->Nodes())
			{
				printf("Node ID: %d(0x%p)\n", node.Id(), &node);
			}
			for (const auto& edge : gGraph->Edges())
			{
				printf("Source ID: %d(0x%p)\tTarget ID: %d(0x%p)\n", 
					edge.SourceNode().Id(), &edge.SourceNode(), edge.TargetNode().Id(), &edge.TargetNode());
			}
			printf("\n");
#endif
		}
	}
	else if (e.type == SDL_MOUSEBUTTONUP)
	{
		switch (e.button.button)
		{
		case SDL_BUTTON_LEFT:
			break;
		}
	}

	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_p:
			GraphParser::SaveGraph(*gGraph, (ContentPath + "Test/test001.xml").c_str());
			break;
		case SDLK_l:
		{
			const SDL_MessageBoxButtonData buttons[] = 
			{
				{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,	0,	"Default" },
				{ 0,										1,	"Web" },
				{ 0,										2,	"Test" },
				{ 0,										3,	"City" },
				{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,	4,	"Cancel" },
			};
			const SDL_MessageBoxData messageBoxData =
			{
				SDL_MESSAGEBOX_INFORMATION,
				nullptr,
				"Load Level",
				"Choose a level to load.",
				SDL_arraysize(buttons),
				buttons,
				nullptr
			};
			std::int32_t buttonId = 0;
			if (SDL_ShowMessageBox(&messageBoxData, &buttonId) >= 0)
			{
				switch (buttonId)
				{
				case 0:
					GraphParser::LoadGraph(gGraph, (ContentPath + "Maps/default.xml").c_str());
					break;
				case 1:
					GraphParser::LoadGraph(gGraph, (ContentPath + "Maps/web.xml").c_str());
					break;
				case 2:
					GraphParser::LoadGraph(gGraph, (ContentPath + "Test/test001.xml").c_str());
					break;
				case 3:
					GraphParser::LoadGraph(gGraph, (ContentPath + "Maps/city.xml").c_str());
					break;
				}
			}
			
			break;
		}
		case SDLK_SPACE:
		{
			// For debugging only
			// Recalculate path of selected vehicle
			if (gSelectedVehicle)
			{
				srand(static_cast<uint32_t>(std::time(NULL)));
				std::int32_t randomIndex = rand() % gGraph->GetNodeCount();
				gGraph->RemoveHighlight(gSelectedVehicle->Itinerary());
				gSelectedVehicle->NavigateTo(*gGraph->GetNodeById(randomIndex));
				gGraph->HighlightPath(gSelectedVehicle->Itinerary());
			}
			break;
		}
		case SDLK_w:
		{
			// Place a wall at the mouse position
			std::int32_t x, y;
			SDL_GetMouseState(&x, &y);
			TrafficSimulator::Wall wall(TextureManager::GetTexture("wall"), Vector2f{ static_cast<float>(x), static_cast<float>(y) });
			gWallList.emplace_back(wall);
			break;
		}
		case SDLK_s:
		{
			// Place a spawner at the mouse position
			if (gGraph->Nodes().size() > 0) 
			{
				std::int32_t x, y;
				SDL_GetMouseState(&x, &y);
				TrafficSimulator::Spawner spawner(TextureManager::GetTexture("spawner"), Vector2f{ static_cast<float>(x), static_cast<float>(y) }, *gGraph);
				gSpawnerList.emplace_back(spawner);
			}
			break;
		}
		}
	}
}

void Update(std::uint32_t delta)
{
	// Calculate the frames processed per second
	gFpsTime += delta;
	if (gFpsTime <= 1000)
	{
		++gFrameCount;
	}
	else
	{
		gPreviousFrameCount = gFrameCount;
		gFrameCount = 0;
		gFpsTime = 0;	
	}
	
	if (TrafficSimulator::Vehicle::TotalVehicleCount() < VehicleCap)
	{
		for (auto& spawner : gSpawnerList)
		{
			spawner.Update(delta);
		}
	}

	for (auto& vehicle : TrafficSimulator::Spawner::Vehicles())
	{
		vehicle.Update(delta, TrafficSimulator::Spawner::Vehicles(), gIntersectionManagers, gWallList);
	}

	for (auto& manager : gIntersectionManagers)
	{
		manager.Update(delta);
	}
}

void Render()
{
	// Clear screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	// Render the graph
	gGraph->Draw(gRenderer);

	// Render each traffic signal
	for (auto& manager : gIntersectionManagers)
	{
		for (auto signal : manager.GetTrafficSignals()) 
		{
			signal.Draw(gRenderer);
		}
	}

	for (auto& vehicle : TrafficSimulator::Spawner::Vehicles())
	{
		vehicle.Draw(gRenderer);
	}

	// Render all walls
	for (auto& wall : gWallList)
	{
		wall.Draw(gRenderer);
	}

	// Render all spawners
	for (auto& spawner : gSpawnerList)
	{
		spawner.Draw(gRenderer);
	}

	TextureManager::RenderText(gRenderer, "calibri", "FPS: " +
		std::to_string(gPreviousFrameCount), Vector2{ 0, 0 });

	TextureManager::RenderText(gRenderer, "calibri", "Total Vehicles: " +
		std::to_string(TrafficSimulator::Vehicle::TotalVehicleCount()), Vector2{ 0, 20 });

	// Update screen
	SDL_RenderPresent(gRenderer);
}

int main(int argc, char* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	bool isRunning = true;
	SDL_Event e;

	if (!InitializeSDL())
	{
		fprintf(ERR_STREAM, "Failed to initialize SDL!\n");
	}
	else
	{
		if (!LoadResources())
		{
			fprintf(ERR_STREAM, "Failed to load resources!\n");
		}
	}

	std::uint32_t previousTick = 0;
	std::uint32_t delta = 0;

	// Create the graph
	gGraph = new Graph(gRenderer, gCalibriFont);

	// Load the default graph
	GraphParser::LoadGraph(gGraph, (ContentPath + "Test/test001.xml").c_str());

	SDL_Texture* signalTexture = TextureManager::GetTexture("signal");

	// Build the intersection signals
	TrafficSimulator::IntersectionManager intersectionManager(5000);
	intersectionManager.AddSignal(signalTexture, gGraph->GetNodeById(0)->Position());
	intersectionManager.AddSignal(signalTexture, gGraph->GetNodeById(2)->Position());
	intersectionManager.AddSignal(signalTexture, gGraph->GetNodeById(4)->Position());
	intersectionManager.AddSignal(signalTexture, gGraph->GetNodeById(6)->Position());
	gIntersectionManagers.emplace_back(intersectionManager);

	TrafficSimulator::IntersectionManager intersectionManager2(5000);
	intersectionManager2.AddSignal(signalTexture, gGraph->GetNodeById(8)->Position());
	intersectionManager2.AddSignal(signalTexture, gGraph->GetNodeById(11)->Position());
	intersectionManager2.AddSignal(signalTexture, gGraph->GetNodeById(13)->Position());
	gIntersectionManagers.emplace_back(intersectionManager2);

	TrafficSimulator::IntersectionManager intersectionManager3(5000);
	intersectionManager3.AddSignal(signalTexture, gGraph->GetNodeById(29)->Position());
	intersectionManager3.AddSignal(signalTexture, gGraph->GetNodeById(31)->Position());
	intersectionManager3.AddSignal(signalTexture, gGraph->GetNodeById(33)->Position());
	gIntersectionManagers.emplace_back(intersectionManager3);

	TrafficSimulator::IntersectionManager intersectionManager4(5000);
	intersectionManager4.AddSignal(signalTexture, gGraph->GetNodeById(26)->Position());
	intersectionManager4.AddSignal(signalTexture, gGraph->GetNodeById(20)->Position());
	intersectionManager4.AddSignal(signalTexture, gGraph->GetNodeById(22)->Position());
	intersectionManager4.AddSignal(signalTexture, gGraph->GetNodeById(24)->Position());
	gIntersectionManagers.emplace_back(intersectionManager4);

	TrafficSimulator::IntersectionManager intersectionManager5(5000);
	intersectionManager5.AddSignal(signalTexture, gGraph->GetNodeById(37)->Position());
	intersectionManager5.AddSignal(signalTexture, gGraph->GetNodeById(39)->Position());
	intersectionManager5.AddSignal(signalTexture, gGraph->GetNodeById(35)->Position());
	gIntersectionManagers.emplace_back(intersectionManager5);

	srand((unsigned)time(0));

	while (isRunning)
	{
		std::uint32_t currentTick = SDL_GetTicks();
		if (currentTick > previousTick)
		{
			delta = currentTick - previousTick;
			previousTick = currentTick;
		}

		while (SDL_PollEvent(&e) != 0)
		{
			HandleInput(e, isRunning);
		}

		Update(delta);

		Render();
	}

	delete gGraph;

	Shutdown();

	return 0;
}
