#pragma once

#include <cstdint>
#include <memory>
#include <list>
#include <SDL.h>
#include "IDrawable.h"
#include "../Engine/Utils.h"

class Node : public TrafficSimulator::IDrawable
{
public:
	Node(SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, std::uint32_t id, const Vector2& position);
	Node(const Node& rhs);
	Node& operator=(const Node& rhs);
	bool operator==(const Node& rhs) const;
	bool operator!=(const Node& rhs) const;
	~Node();
	void InitializeForPathfinding();
	void Draw(SDL_Renderer* renderer);
	const Vector2& Position() const;
	std::uint32_t Id() const;
	inline const std::list<const Node*>& AdjacentNodes() const { return mAdjacentNodes; }
	void AddAdjacentNode(const Node& node) { mAdjacentNodes.emplace_back(&node); }
	inline double MovementCost() const { return mMovementCost; }
	void SetMovementCost(double newCost) { mMovementCost = newCost; }
	inline const Node* Parent() const { return mParent; }
	void SetParent(const Node& parent) { mParent = &parent; }
	static std::uint32_t Count();
	static void ResetCount();

private:
	static std::uint32_t sTotalNodes;
	static std::int32_t sWidth;
	static std::int32_t sHeight;
	SDL_Texture* mTexture;
	SDL_Texture* mIdTexture;
	TTF_Font* mFont;
	SDL_Rect mBoundingRect;
	std::uint32_t mNodeId;
	Vector2 mPosition;
	std::list<const Node*> mAdjacentNodes;
	
	// Used for pathfinding algorithms
	const Node* mParent;
	double mMovementCost;
	double mHeuristic;
	double mTotalCost;
};
