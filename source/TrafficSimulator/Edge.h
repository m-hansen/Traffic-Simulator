#pragma once

#include <SDL.h>
#include "../Engine/Utils.h"

class Node;

class Edge : TrafficSimulator::IDrawable
{
private:
	enum class RoadType
	{
		Invalid,
		Concrete,
		Dirt
	};

	enum class RoadQuality
	{
		Invalid,
		Low,
		Average,
		High
	};

public:
	Edge(const Node& source, const Node& target);
	Edge(const Edge& rhs);
	Edge& operator=(const Edge& rhs);
	~Edge();
	void Draw(SDL_Renderer* renderer);
	const Node& SourceNode() const;
	const Node& TargetNode() const;

private:
	const Node* mSourceNode;
	const Node* mTargetNode;
	double mWeight;
};

