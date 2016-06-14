#pragma once

#include <string>
#include "tinyxml2.h"

using namespace tinyxml2;

class Graph;

class GraphParser
{
public:
	GraphParser() = default;
	~GraphParser() = default;
	static bool LoadGraph(Graph* graph, const std::string& filename);
	static bool SaveGraph(const Graph& graph, const std::string& filename);
};
