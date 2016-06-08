#pragma once

#include <cstdint>
#include <SDL.h>
#include "Node.h"
#include "Edge.h"

class Graph : TrafficSimulator::IDrawable
{
public:
	Graph();
	~Graph();
	void Draw(SDL_Renderer* renderer);
	void CreateEdge(Node* from, Node* to);
	//void CreateNode(const Vector2f& position);
	//const Node& GetNode(std::int32_t id);
	//const Edge& GetEdge(std::int32_t fromNodeId, std::int32_t toNodeId);

private:
	std::vector<Node> mNodeList;
	std::vector<Edge> mEdgeList;
};
