#include "pch.h"

Graph::Graph(SDL_Renderer* renderer, TTF_Font* font)
	: mRenderer(renderer), mFont(font)
{
}

Graph::~Graph()
{
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

void Graph::CreateEdge(const Node& from, const Node& to)
{
	Edge edge(from, to);
	mEdgeList.emplace_back(edge);
}

void Graph::CreateNode(const Vector2f& position)
{
	Node node(mRenderer, TextureManager::GetTexture("node"), mFont, position);
	mNodeList.emplace_back(node);
}

std::uint32_t Graph::GetNodeCount() const
{
	return mNodeList.size();
}

const Node* Graph::GetNodeById(std::uint32_t id)
{
	for (const auto& node : mNodeList)
	{
		if (node.GetId() == id)
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
