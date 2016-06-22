#include "pch.h"

#define ERR_STREAM stderr

const std::uint32_t ScreenWidth = 1280;
const std::uint32_t ScreenHeight = 720;
const std::string ContentPath = "../../Content/";

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
TTF_Font* gCalibriFont = nullptr;
Graph* gGraph = nullptr;

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
	success = TextureManager::LoadTexture(gRenderer, "node", ContentPath + "Images/node.png");

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
			// TODO: Select a node
		}
		if (e.button.button == SDL_BUTTON_RIGHT)
		{
			gGraph->CreateNode(Node::Count(), Vector2{ e.button.x, e.button.y });

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
		}
		if (e.button.button == SDL_BUTTON_MIDDLE)
		{
			// TODO: Remove a node
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
			GraphParser::LoadGraph(gGraph, (ContentPath + "Test/test001.xml").c_str());
			break;
		}
	}
}

void Update(std::uint32_t delta)
{
}

void Render()
{
	// Clear screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	// Render the graph
	gGraph->Draw(gRenderer);

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
