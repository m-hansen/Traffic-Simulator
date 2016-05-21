#include "pch.h"

const std::uint32_t ScreenWidth = 640;
const std::uint32_t ScreenHeight = 480;

SDL_Window* gWindow = nullptr;
SDL_Surface* gScreenSurface = nullptr;
SDL_Surface* gHelloWorld = nullptr;

bool InitializeSDL()
{
	bool success = true;

	// Initialize SDL and create a window
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("Traffic Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);
		if (gWindow == nullptr)
		{
			fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return success;
}

bool LoadResources()
{
	bool success = true;

	gHelloWorld = SDL_LoadBMP("../Content/Images/traffic_simulator.bmp");
	if (gHelloWorld == nullptr)
	{
		fprintf(stderr, "Unable to load image %s! SDL Error: %s\n", "traffic_simulator.bmp", SDL_GetError());
		success = false;
	}

	return success;
}

void Shutdown()
{
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = nullptr;

	// Destroy window will handle the destruction of the screen surface
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	SDL_Quit();
}

void HandleInput(const SDL_Event& e, bool& isRunning)
{
	if (e.type == SDL_QUIT)
	{
		isRunning = false;
	}
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
		fprintf(stderr, "Failed to initialize SDL!\n");
	}
	else
	{
		if (!LoadResources())
		{
			fprintf(stderr, "Failed to load resources!\n");
		}
	}
	
	while (isRunning) 
	{
		while (SDL_PollEvent(&e) != 0)
		{
			HandleInput(e, isRunning);

			SDL_BlitSurface(gHelloWorld, nullptr, gScreenSurface, nullptr);
			SDL_UpdateWindowSurface(gWindow);
		}
	}

	Shutdown();

	return 0;
}