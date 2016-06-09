#include "pch.h"

#define ERR_STREAM stderr

const std::uint32_t ScreenWidth = 1280;
const std::uint32_t ScreenHeight = 720;
const std::string ContentPath = "../Content/";

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
TTF_Font* gCalibriFont = nullptr;
Graph* gGraph = nullptr;

std::vector<TrafficSimulator::Vehicle> gVehicleList;
std::vector<TrafficSimulator::Wall> gWallList;

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
		TextureManager::LoadTexture(gRenderer, "car", ContentPath + "Images/car.png") && 
		TextureManager::LoadTexture(gRenderer, "wall", ContentPath + "Images/wall.png") &&
		TextureManager::LoadTexture(gRenderer, "node", ContentPath + "Images/node.png");

	// Load the calibri font
	const std::uint8_t CalibriFontSize = 24;
	gCalibriFont = TTF_OpenFont((ContentPath + "Fonts/calibri.ttf").c_str(), CalibriFontSize);
	if (gCalibriFont == nullptr)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

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
			// Place a wall at the mouse position
			TrafficSimulator::Wall wall(TextureManager::GetTexture("wall"), Vector2f(static_cast<float>(e.button.x), static_cast<float>(e.button.y)));
			gWallList.emplace_back(wall);
		}
		if (e.button.button == SDL_BUTTON_RIGHT)
		{
			gGraph->CreateNode(Vector2f(static_cast<float>(e.button.x), static_cast<float>(e.button.y)));

			// Temporary code for testing - creates edges between current and last nodes placed
			std::uint32_t graphNodeCount = gGraph->GetNodeCount();
			if (graphNodeCount > 1)
			{
				gGraph->CreateEdge(*gGraph->GetNodeAtIndex(graphNodeCount - 2), *gGraph->GetNodeAtIndex(graphNodeCount - 1));
			}
		}
		if (e.button.button == SDL_BUTTON_MIDDLE)
		{
			// Place a vehicle at the mouse position
			const std::int32_t CarWidth = 26;
			const std::int32_t CarHeight = 40;
			TrafficSimulator::Vehicle car(
				TextureManager::GetTexture("car"),
				Vector2f(static_cast<float>(e.button.x), static_cast<float>(e.button.y)),
				CarWidth,
				CarHeight
			);
			gVehicleList.emplace_back(car);
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
}

void Update(std::uint32_t delta)
{
	for (auto& vehicle : gVehicleList)
	{
		vehicle.Update(delta, gVehicleList, gWallList);
	}
}

void Render()
{
	// Clear screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	// Render all vehicles to screen
	for (auto& vehicle : gVehicleList)
	{
		vehicle.Draw(gRenderer);
	}

	// Render all walls
	for (auto& wall : gWallList)
	{
		wall.Draw(gRenderer);
	}

	gGraph->Draw(gRenderer);

	// Update screen
	SDL_RenderPresent(gRenderer);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

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

	const std::int32_t CarWidth = 26;
	const std::int32_t CarHeight = 40;
	TrafficSimulator::Vehicle car(
		TextureManager::GetTexture("car"),
		Vector2f(ScreenWidth / 2, ScreenHeight / 2), 
		CarWidth, 
		CarHeight
	);
	gVehicleList.emplace_back(car);

	std::uint32_t previousTick = 0;
	std::uint32_t delta = 0;

	// Create the graph
	gGraph = new Graph(gRenderer, gCalibriFont);

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

			Update(delta);

			Render();
		}
	}

	delete gGraph;

	Shutdown();

	return 0;
}
