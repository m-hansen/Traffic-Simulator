#include "pch.h"

Edge::Edge(Node* from, Node* to)
	: mNodeFrom(from), mNodeTo(to)
{
}

Edge::~Edge()
{
}

void Edge::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

	Vector2f fromPosition = mNodeFrom->GetPosition();
	Vector2f toPosition = mNodeTo->GetPosition();
	SDL_RenderDrawLine
	(
		renderer,
		static_cast<std::int32_t>(fromPosition.x),
		static_cast<std::int32_t>(fromPosition.y),
		static_cast<std::int32_t>(toPosition.x),
		static_cast<std::int32_t>(toPosition.y)
	);
}
