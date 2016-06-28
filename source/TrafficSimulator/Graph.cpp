#include "pch.h"

const SDL_Color Graph::sDefaultEdgeColor = SDL_Color{ 0x00, 0x00, 0x00, 0xFF };

Graph::Graph(SDL_Renderer* renderer, TTF_Font* font)
	: mRenderer(renderer), mFont(font), mNodeList(), mEdgeList()
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
	for (auto& edge : mEdgeList)
	{
		edge.Draw(renderer);
	}

	for (auto& node : mNodeList)
	{
		node.Draw(renderer);
	}
}

void Graph::HighlightPath(const std::list<const Node*>& nodes, const SDL_Color color)
{
	const Node* previousNode = nullptr;

	for (const Node* node : nodes)
	{
		if (previousNode != nullptr)
		{
			// Color edges
			// Note: Both directions, if it exists, are being highlighted for debugging purposes
			const_cast<Edge*>(FindEdge(*previousNode, *node))->SetColor(SDL_Color{ color.r, color.g, color.b, color.a });
			const Edge* temp = FindEdge(*node, *previousNode);
			if (temp)
			{
				const_cast<Edge*>(temp)->SetColor(SDL_Color{ color.r, color.g, color.b, color.a });
			}
		}

		previousNode = node;
	}
}

void Graph::RemoveHighlight(const std::list<const Node*>& nodes)
{
	HighlightPath(nodes, sDefaultEdgeColor);
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

const Node* Graph::GetNodeById(std::uint32_t id) const
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

const Edge* Graph::FindEdge(const Node& source, const Node& target) const
{
	const Edge* foundEdge = nullptr;

	for (std::list<Edge>::const_iterator edge = mEdgeList.begin(); edge != mEdgeList.end(); ++edge)
	{
		if (edge->SourceNode() == source && edge->TargetNode() == target)
		{
			foundEdge = &(*edge);
			break;
		}
	}

	return foundEdge;
}

void Graph::InitializeForPathfinding()
{
	for (Node& node : mNodeList)
	{
		node.InitializeForPathfinding();
	}
}
