#include "pch.h"

Pathfinder::Pathfinder()
{
}

Pathfinder::~Pathfinder()
{
}

std::list<const Node*> Pathfinder::Dijkstras(const Graph& graph, const Node& source, const Node& target)
{
	std::set<const Node*> openSet;
	std::set<const Node*> closedSet;

	// Initialize the graph
	const_cast<Graph&>(graph).InitializeForPathfinding();

	openSet.insert(&source);

	while (!openSet.empty())
	{
		const Node* temp = Pathfinder::GetLowestCostNode(openSet);
		const Node* currentNode = graph.GetNodeById(temp->Id());
		closedSet.insert(temp); 
		openSet.erase(temp);

		if (currentNode->Id() == target.Id())
		{
			// Early out when target reached
			break;
		}

		for (const Node* neighbor : currentNode->AdjacentNodes())
		{
			// Calculate the cost
			const Edge* edge = graph.FindEdge(*currentNode, *neighbor);
			assert(edge);
			double pathCost = edge->Weight();

			// Check if closed set doesn't contain neighbor or if movement cost is lower
			bool closedSetContainsNeighbor = (closedSet.find(neighbor) != closedSet.end());
			if (closedSetContainsNeighbor) continue;
			if (!closedSetContainsNeighbor || pathCost < neighbor->MovementCost())
			{
				// Update the movement cost, set parent and add to the closed list
				const_cast<Node*>(neighbor)->SetMovementCost(pathCost);
				closedSet.insert(neighbor);
				const_cast<Node*>(neighbor)->SetParent(*currentNode);
				openSet.insert(neighbor);
			}
		}
	}

	// Build the shortest path
	std::list<const Node*> shortestPath;
	const Node* shortestPathNode = &target;
	while (shortestPathNode != nullptr)
	{
		shortestPath.emplace_back(shortestPathNode);
		shortestPathNode = shortestPathNode->Parent();
	}
	shortestPath.reverse();

#if (_DEBUG)
	// Display the shortest path in the console
	printf("Path (Dijkstra's): ");
	for (const Node* node : shortestPath)
	{
		printf("%d ", node->Id());
	}
	printf("\n");
#endif

	return shortestPath;
}

const Node* Pathfinder::GetLowestCostNode(const std::set<const Node*>& set)
{
	assert(!set.empty());
	const Node* nodeWithLowestCost = *set.begin();

	for (const Node* node : set)
	{
		if (node->MovementCost() < nodeWithLowestCost->MovementCost())
		{
			nodeWithLowestCost = node;
		}
	}

	assert(nodeWithLowestCost);
	return nodeWithLowestCost;
}