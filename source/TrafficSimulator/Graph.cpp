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

void Graph::CreateNode(const Vector2& position)
{
	Node node(mRenderer, TextureManager::GetTexture("node"), mFont, position);
	mNodeList.emplace_back(node);
}

std::uint32_t Graph::GetNodeCount() const
{
	// Note: The number of nodes in a graph must never exceed the maximum value of a 32-bit unsigned integer
	return static_cast<std::uint32_t>(mNodeList.size());
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

const Node* Graph::GetNodeAtIndex(std::uint32_t index) const
{
	if (index < GetNodeCount())
	{
		return &mNodeList.at(index);
	}

	return nullptr;
}

const std::vector<Node>& Graph::Nodes() const
{
	return mNodeList;
}

const std::vector<Edge>& Graph::Edges() const
{
	return mEdgeList;
}