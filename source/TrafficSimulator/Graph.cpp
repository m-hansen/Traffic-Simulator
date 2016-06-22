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
	for (auto& edge : mEdgeList)
	{
		edge.Draw(renderer);
	}

	for (auto& node : mNodeList)
	{
		node.Draw(renderer);
	}
}

void Graph::HighlightPath(const std::list<const Node*>& nodes)
{
	const Node* previousNode = nullptr;

	for (const Node* node : nodes)
	{
		if (previousNode != nullptr)
		{
			// Color edges
			// Note: Both directions, if it exists, are being highlighted for debugging purposes
			const_cast<Edge*>(FindEdge(*previousNode, *node))->SetColor(SDL_Color{ 0xFF, 0x00, 0x00, 0xFF });
			const Edge* temp = FindEdge(*node, *previousNode);
			if (temp)
			{
				const_cast<Edge*>(temp)->SetColor(SDL_Color{ 0xFF, 0x00, 0x00, 0xFF });
			}
		}

		previousNode = node;
	}

	//for (std::uint32_t i = 0; i < nodes.size(); ++i)
	//{
	//	Node* currentNode = nodes.front;
	//	assert(currentNode);

	//	//currentNode->Draw(renderer);

	//	if (i == 0)
	//	{
	//		// Color start node
	//	}
	//	else
	//	{
	//		// Color edges
	//		const_cast<Edge*>(FindEdge(*GetNodeById(i - 1), *currentNode))->SetColor(SDL_Color{ 0xFF, 0x00, 0x00, 0xFF });
	//	}

	//	currentNode = currentNode->
	//}

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
