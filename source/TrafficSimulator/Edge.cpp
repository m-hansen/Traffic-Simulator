#include "pch.h"

Edge::Edge(const Node& source, const Node& target)
	: mSourceNode(source), mTargetNode(target), mWeight(0.0)
{
}

Edge::~Edge()
{
}

void Edge::Draw(SDL_Renderer* renderer)
{
	Vector2 sourcePosition = mSourceNode.Position();
	Vector2 targetPosition = mTargetNode.Position();
	SDL_RenderDrawLine(renderer, sourcePosition.x, sourcePosition.y, targetPosition.x, targetPosition.y);
}

const Node& Edge::SourceNode() const
{
	return mSourceNode;
}

const Node& Edge::TargetNode() const
{
	return mTargetNode;
}