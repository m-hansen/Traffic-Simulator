#pragma once

#include <deque>
#include "Node.h"

class Graph;

class Pathfinder
{
public:
	Pathfinder();
	~Pathfinder();
	static std::list<const Node*> Dijkstras(const Graph& graph, const Node& source, const Node& target);

private:
	static const Node* GetLowestCostNode(const std::set<const Node*>& set);
};

