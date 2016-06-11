#include "pch.h"

std::map<std::string, SDL_Texture*> TextureManager::textureMap;

bool TextureManager::LoadTexture(SDL_Renderer* renderer, std::string referenceName, std::string location)
{
	SDL_Surface* surface = NULL;

	if (location.substr(location.length() - 3, location.length()) == "bmp")
	{
		// Load bitmaps
		surface = SDL_LoadBMP(location.c_str());

		// Set any magenta regions to alpha
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0, 0xFF));

		if (!surface)
		{
			printf("Could not load bitmap! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
	}
	else
	{
		// Load other image types
		surface = IMG_Load(location.c_str());

		// Set any magenta regions to alpha
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0, 0xFF));

		if (!surface)
		{
			printf("Could not load image! IMG_Error: %s\n", IMG_GetError());
			return false;
		}
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	// Create and add to map if successful
	textureMap.insert(std::map<std::string, SDL_Texture*>::value_type(referenceName.c_str(), texture));
	SDL_FreeSurface(surface);

	return true;
}

void TextureManager::UnloadTexture(std::string referenceName)
{
	// Free the memory for an individual resource
	std::map<std::string, SDL_Texture*>::iterator iter = textureMap.find(referenceName);
	SDL_DestroyTexture(iter->second);
	textureMap.erase(iter);
}

void TextureManager::ClearAll()
{
	for (auto texture : textureMap)
	{
		if (texture.second != nullptr)
		{
			// Destroy each texture in the map
			SDL_DestroyTexture(texture.second);
			texture.second = nullptr;
		}
	}
	textureMap.clear();
}

SDL_Texture* TextureManager::GetTexture(std::string key)
{
	auto iter = textureMap.find(key);

	// Validate the key
	if (iter == textureMap.end())
	{
		fprintf(stdout, "Could not find the texture with the key \"%s\" - returning NULL\n", key.c_str());
		return nullptr;
	}

	return iter->second;
}
