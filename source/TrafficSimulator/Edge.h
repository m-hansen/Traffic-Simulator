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
	Edge(const Node& from, const Node& to);
	Edge(const Edge& rhs);
	Edge& operator=(const Edge& rhs);
	~Edge();
	void Draw(SDL_Renderer* renderer);

private:
	const Node* mNodeFrom;
	const Node* mNodeTo;
	double mCost;
};

