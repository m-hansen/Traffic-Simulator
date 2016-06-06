#include "pch.h"

std::uint32_t Node::sTotalNodes = 0;
std::int32_t Node::sWidth = 25;
std::int32_t Node::sHeight = 25;

Node::Node(SDL_Texture* texture, const Vector2f& position)
	: mTexture(texture), mNodeId(sTotalNodes++), mPosition(position.x - sWidth / 2, position.y  - sWidth / 2), mNeighborNodes()
{
	mBoundingRect = 
	{
		static_cast<std::int32_t>(mPosition.x),
		static_cast<std::int32_t>(mPosition.y),
		sWidth,
		sHeight
	};
}

Node::~Node()
{
}

void Node::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

	SDL_RenderCopy(renderer, mTexture, nullptr, &mBoundingRect);
}