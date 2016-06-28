#include "pch.h"

std::map<std::string, SDL_Texture*> TextureManager::mTextureMap;
std::map<std::string, TTF_Font*> TextureManager::mFontMap;

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
	mTextureMap.insert(std::map<std::string, SDL_Texture*>::value_type(referenceName.c_str(), texture));
	SDL_FreeSurface(surface);

	return true;
}

void TextureManager::UnloadTexture(std::string referenceName)
{
	// Free the memory for an individual resource
	std::map<std::string, SDL_Texture*>::iterator iter = mTextureMap.find(referenceName);
	SDL_DestroyTexture(iter->second);
	mTextureMap.erase(iter);
}

void TextureManager::ClearAll()
{
	for (auto texture : mTextureMap)
	{
		if (texture.second != nullptr)
		{
			// Destroy each texture in the map
			SDL_DestroyTexture(texture.second);
			texture.second = nullptr;
		}
	}
	mTextureMap.clear();
}

SDL_Texture* TextureManager::GetTexture(std::string key)
{
	auto iter = mTextureMap.find(key);

	// Validate the key
	if (iter == mTextureMap.end())
	{
		fprintf(stdout, "Could not find the texture with the key \"%s\" - returning NULL\n", key.c_str());
		return nullptr;
	}

	return iter->second;
}

bool TextureManager::LoadFont(const std::string& referenceName, const std::string& location, std::uint8_t fontSize)
{
	bool success = true;

	// Load the font
	//const std::uint8_t CalibriFontSize = 24; // TODO: default this param in header
	TTF_Font* font = TTF_OpenFont(location.c_str(), fontSize);
	if (font == nullptr)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		mFontMap.insert(std::map<std::string, TTF_Font*>::value_type(referenceName.c_str(), font));
	}

	return success;
}

void TextureManager::RenderText(SDL_Renderer* renderer, const std::string& fontName, const std::string& text, const Vector2& position)
{
	SDL_Color black = { 0, 0, 0 };
	SDL_Surface* surface = TTF_RenderText_Blended(mFontMap[fontName], text.c_str(), black);
	if (surface != nullptr)
	{
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

		SDL_FreeSurface(surface);
		surface = nullptr;

		if (texture == nullptr)
		{
			printf("Failed to create texture from surface in RenderText()\n");
		}
		else
		{
			std::int32_t width = 0;
			std::int32_t height = 0;
			SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
			SDL_Rect rect = { position.x, position.y, width, height };
			SDL_RenderCopy(renderer, texture, nullptr, &rect);
			SDL_DestroyTexture(texture);
		}
	}
}

// TODO unload fonts
// TODO remove fonts on clear
