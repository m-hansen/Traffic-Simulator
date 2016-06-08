#pragma once

#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace Engine
{
	class TextureManager
	{
	public:
		static bool LoadTexture(SDL_Renderer* renderer, std::string referenceName, std::string location);
		static SDL_Texture* GetTexture(std::string key);
		static void UnloadTexture(std::string referenceName);
		static void ClearAll();

	private:
		static std::map<std::string, SDL_Texture*> textureMap;
	};
}