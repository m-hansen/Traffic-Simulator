#pragma once

#include <SDL.h>
#include "../Engine/Utils.h"

class Node;

class Edge : public TrafficSimulator::IDrawable
{
private:
	enum class RoadType
	{
		Invalid,
		Concrete,
		Dirt
	};

	enum class RoadQuality
	{
		Invalid,
		Low,
		Average,
		High
	};

public:
	Edge(const Node& source, const Node& target);
	bool operator==(const Edge& rhs) const;
	bool operator!=(const Edge& rhs) const;
	~Edge();
	void Edge::Draw(SDL_Renderer* renderer);
	const Node& SourceNode() const;
	const Node& TargetNode() const;
	double Weight() const;
	inline void SetColor(const SDL_Color& color) { mColor = color; }
	inline void SetColor(std::uint8_t r, std::uint8_t g, std::uint8_t b) { mColor = SDL_Color{ r, g, b, 0xFF }; }

private:
	const Node& mSourceNode;
	const Node& mTargetNode;
	double mWeight;
	SDL_Color mColor;
	std::int32_t mNumLanes;
};
