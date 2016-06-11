#include "pch.h"

Edge::Edge(const Node& from, const Node& to)
	: mNodeFrom(&from), mNodeTo(&to), mCost(0.0)
{
}

Edge::Edge(const Edge& rhs)
	: mNodeFrom(rhs.mNodeFrom), mNodeTo(rhs.mNodeTo), mCost(rhs.mCost)
{
}

Edge& Edge::operator=(const Edge& rhs)
{
	if (this != &rhs)
	{
		mNodeFrom = rhs.mNodeFrom;
		mNodeTo = rhs.mNodeTo;
		mCost = rhs.mCost;
	}

	return *this;
}

Edge::~Edge()
{
}

void Edge::Draw(SDL_Renderer* renderer)
{
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
