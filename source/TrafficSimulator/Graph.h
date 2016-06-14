#pragma once

#include <cstdint>
#include <memory>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Node.h"
#include "Edge.h"

class Graph : TrafficSimulator::IDrawable
{
public:
	Graph(SDL_Renderer* renderer, TTF_Font* font);
	~Graph();
	void Clear();
	void Draw(SDL_Renderer* renderer);
	void CreateEdge(const Node& source, const Node& target);
	void CreateNode(const Vector2& position);
	std::uint32_t GetNodeCount() const;
	const Node* GetNodeById(std::uint32_t id);
	const Node* GetNodeAtIndex(std::uint32_t index) const;
	const std::vector<Node>& Nodes() const;
	const std::vector<Edge>& Edges() const;

private:
	std::vector<Node> mNodeList;
	std::vector<Edge> mEdgeList;
	SDL_Renderer* mRenderer;
	TTF_Font* mFont;
};
