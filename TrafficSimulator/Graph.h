#pragma once

#include "Node.h"

class Graph
{
public:
	Graph();
	~Graph();

private:
	std::vector<Node> mNodeList;
};
