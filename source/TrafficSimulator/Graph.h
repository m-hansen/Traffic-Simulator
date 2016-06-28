#pragma once

#include <cstdint>
#include <memory>
#include <list>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Node.h"
#include "Edge.h"

class Graph : TrafficSimulator::IDrawable
{
public:
	Graph(SDL_Renderer* renderer, TTF_Font* font);
	~Graph() = default;
	void Clear();
	void Draw(SDL_Renderer* renderer);
	void HighlightPath(const std::list<const Node*>& nodes, SDL_Color color = SDL_Color{ 0x00,0x72,0xB2,0xFF });
	void RemoveHighlight(const std::list<const Node*>& nodes);
	void CreateEdge(const Node& source, const Node& target);
	void CreateNode(std::uint32_t id, const Vector2& position);
	std::uint32_t GetNodeCount() const;
	const Node* GetNodeById(std::uint32_t id) const;
	const std::list<Node>& Nodes() const;
	const std::list<Edge>& Edges() const;
	const Edge* FindEdge(const Node& source, const Node& target) const;
	void InitializeForPathfinding();

private:
	static const SDL_Color sDefaultEdgeColor;
	std::list<Node> mNodeList;
	std::list<Edge> mEdgeList;
	SDL_Renderer* mRenderer;
	TTF_Font* mFont;
};
