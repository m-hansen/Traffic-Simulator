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
	Edge(Node* from, Node* to);
	~Edge();
	void Draw(SDL_Renderer* renderer);

private:
	Node* mNodeFrom;
	Node* mNodeTo;
	double mCost;
};

