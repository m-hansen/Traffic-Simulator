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
	Node(SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, const Vector2f& position);
	~Node();
	void Draw(SDL_Renderer* renderer);
	const Vector2f& GetPosition() const;
	std::uint32_t GetId() const;

private:
	static std::uint32_t sTotalNodes;
	static std::int32_t sWidth;
	static std::int32_t sHeight;
	SDL_Texture* mTexture;
	SDL_Texture* mIdTexture;
	TTF_Font* mFont;
	SDL_Rect mBoundingRect;
	std::uint32_t mNodeId;
	Vector2f mPosition;
	std::vector<Node*> mAdjacentNodes;
};
