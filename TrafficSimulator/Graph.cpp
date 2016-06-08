#include "pch.h"

Graph::Graph()
{
}

Graph::~Graph()
{
}

// TODO: Look into batch rendering to speed this process up
void Graph::Draw(SDL_Renderer* renderer)
{
	for (auto& node : mNodeList)
	{
		node.Draw(renderer);
	}

	for (auto& edge : mEdgeList)
	{
		edge.Draw(renderer);
	}
}

void Graph::CreateEdge(Node* from, Node* to)
{
	Edge newEdge(from, to);
	mEdgeList.emplace_back(newEdge);
}

//const Node& Graph::GetNode(std::int32_t id)
//{
//}

//const Edge& Graph::GetEdge(std::int32_t fromNodeId, std::int32_t toNodeId)
//{
//
//}
