#include "pch.h"

Edge::Edge(const Node& source, const Node& target)
	: mSourceNode(source), mTargetNode(target), mWeight(0.0), mColor(SDL_Color{0x00, 0x00, 0x00, 0xFF})
{
	// Distance is squared
	double distanceSq = std::pow((source.Position().x - target.Position().x), 2) +
		std::pow((source.Position().y - target.Position().y), 2);

	mWeight = distanceSq; // For now, the total weight can just equal the distance squared

#if (_DEBUG)
	printf("Edge %d->%d weight : %.2lf\n", source.Id(), target.Id(), mWeight);
#endif
}

bool Edge::operator==(const Edge& rhs) const
{
	return ((mSourceNode == rhs.SourceNode()) && (mTargetNode == rhs.TargetNode()));
}

bool Edge::operator!=(const Edge& rhs) const
{
	return !(*this == rhs);
}

Edge::~Edge()
{
}

void Edge::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
	Vector2 sourcePosition = mSourceNode.Position();
	Vector2 targetPosition = mTargetNode.Position();
	SDL_RenderDrawLine(renderer, sourcePosition.x, sourcePosition.y, targetPosition.x, targetPosition.y);

	// TODO: remove this - for presentation only after complaints of being unable to see debugging info
	SDL_RenderDrawLine(renderer, sourcePosition.x - 1, sourcePosition.y - 1, targetPosition.x - 1, targetPosition.y - 1);
	SDL_RenderDrawLine(renderer, sourcePosition.x + 1, sourcePosition.y + 1, targetPosition.x + 1, targetPosition.y + 1);
	SDL_RenderDrawLine(renderer, sourcePosition.x - 2, sourcePosition.y - 2, targetPosition.x - 2, targetPosition.y - 2);
	SDL_RenderDrawLine(renderer, sourcePosition.x + 2, sourcePosition.y + 2, targetPosition.x + 2, targetPosition.y + 2);
}

const Node& Edge::SourceNode() const
{
	return mSourceNode;
}

const Node& Edge::TargetNode() const
{
	return mTargetNode;
}

double Edge::Weight() const
{ 
	return mWeight;
}