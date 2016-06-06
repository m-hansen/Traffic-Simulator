#pragma once

#include <cstdint>
#include <vector>
#include <SDL.h>
#include "IDrawable.h"
#include "../Engine/Utils.h"

class Node : public TrafficSimulator::IDrawable
{
public:
	Node(SDL_Texture* texture, const Vector2f& position);
	~Node();
	void Draw(SDL_Renderer* renderer);

private:
	static std::uint32_t sTotalNodes;
	static std::int32_t sWidth;
	static std::int32_t sHeight;
	SDL_Texture* mTexture;
	SDL_Rect mBoundingRect;
	std::uint32_t mNodeId;
	Vector2f mPosition;
	std::vector<Node*> mNeighborNodes;
};
