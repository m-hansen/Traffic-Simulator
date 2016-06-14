#include "pch.h"

Graph::Graph(SDL_Renderer* renderer, TTF_Font* font)
	: mRenderer(renderer), mFont(font), mNodeList(), mEdgeList()
{
}

Graph::~Graph()
{
}

void Graph::Clear()
{
	mEdgeList.clear();
	mNodeList.clear();
	Node::ResetCount();
}

// TODO: Look into batch rendering to speed this process up
void Graph::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	for (auto& node : mNodeList)
	{
		node.Draw(renderer);
	}

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	for (auto& edge : mEdgeList)
	{
		edge.Draw(renderer);
	}
}

void Graph::CreateEdge(const Node& source, const Node& target)
{
	Edge edge(source, target);
	mEdgeList.emplace_back(edge);
}

void Graph::CreateNode(std::uint32_t id, const Vector2& position)
{
	Node node(mRenderer, TextureManager::GetTexture("node"), mFont, id, position);
	mNodeList.emplace_back(node);
}

std::uint32_t Graph::GetNodeCount() const
{
	// Note: The number of nodes in a graph must never exceed the maximum value of a 32-bit unsigned integer
	return Node::Count();
}

const Node* Graph::GetNodeById(std::uint32_t id)
{
	for (const auto& node : mNodeList)
	{
		if (node.Id() == id)
		{
			return &node;
		}
	}

	return nullptr;
}

const std::list<Node>& Graph::Nodes() const
{
	return mNodeList;
}

const std::list<Edge>& Graph::Edges() const
{
	return mEdgeList;
}