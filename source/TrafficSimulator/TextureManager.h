#pragma once

#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

class TextureManager
{
public:
	static bool LoadTexture(SDL_Renderer* renderer, std::string referenceName, std::string location);
	static SDL_Texture* GetTexture(std::string key);
	static void UnloadTexture(std::string referenceName);
	static void ClearAll();
	static bool LoadFont(const std::string& referenceName, const std::string& location, std::uint8_t fontSize);
	static void RenderText(SDL_Renderer* renderer, const std::string& fontName, const std::string& text, const SDL_Rect& rect);

private:
	static std::map<std::string, SDL_Texture*> mTextureMap;
	static std::map<std::string, TTF_Font*> mFontMap;
};
