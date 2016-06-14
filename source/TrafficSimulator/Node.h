#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <SDL.h>
#include "IDrawable.h"
#include "../Engine/Utils.h"

class Node : public TrafficSimulator::IDrawable
{
public:
	Node(SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, const Vector2& position);
	Node(const Node& rhs);
	Node& operator=(const Node& rhs);	
	~Node();
	void Draw(SDL_Renderer* renderer);
	const Vector2& Position() const;
	std::uint32_t Id() const;

private:
	static std::uint32_t sTotalNodes;
	static std::int32_t sWidth;
	static std::int32_t sHeight;
	SDL_Texture* mTexture;
	SDL_Texture* mIdTexture;
	TTF_Font* mFont;
	SDL_Rect mBoundingRect;
	std::uint32_t mNodeId;
	Vector2 mPosition;
	std::vector<Node*> mAdjacentNodes;
};
