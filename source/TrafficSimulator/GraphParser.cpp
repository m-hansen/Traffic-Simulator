#include "pch.h"

bool GraphParser::LoadGraph(Graph* graph, const std::string& filename)
{
	tinyxml2::XMLDocument xmlDocument;

	if (xmlDocument.LoadFile(filename.c_str()) != 0)
	{
		fprintf(stderr, "Error: The GraphParser failed to load %s.\nError Description: %s\n", filename.c_str(), xmlDocument.ErrorName());
		return false;
	}

	XMLNode* root = xmlDocument.FirstChild();

	if (root == nullptr)
	{
		fprintf(stderr, "Error: The GraphParser failed to read the root element of the XML document %s.\n", filename.c_str());
		return false;
	}

	XMLElement* element = root->FirstChildElement("graph");
	if (element == nullptr)
	{
		fprintf(stderr, "Error: The GraphParser failed to read the graph element of the XML document %s.\n", filename.c_str());
		return false;
	}

	// Clear the previous graph before constructing the new one
	graph->Clear();

	// Construct the graph nodes
	XMLElement* nodeElement = element->FirstChildElement("node");
	while (nodeElement != nullptr)
	{
		std::int32_t id = -1;
		std::int32_t xPosition = -1;
		std::int32_t yPosition = -1;

		nodeElement->QueryIntAttribute("id", &id);
		nodeElement->QueryIntAttribute("x", &xPosition);
		nodeElement->QueryIntAttribute("y", &yPosition);

		graph->CreateNode(id, Vector2{ xPosition, yPosition});
		nodeElement = nodeElement->NextSiblingElement("node");
	}

	// Construct the graph edges
	XMLElement* edgeElement = element->FirstChildElement("edge");
	while (edgeElement != nullptr)
	{
		std::int32_t sourceNodeId = 0;
		std::int32_t targetNodeId = 0;
		bool isVisible = true;

		edgeElement->QueryIntAttribute("source", &sourceNodeId);
		edgeElement->QueryIntAttribute("target", &targetNodeId);
		edgeElement->QueryBoolAttribute("visible", &isVisible);

		const Node* sourceNode = graph->GetNodeById(sourceNodeId);
		const Node* targetNode = graph->GetNodeById(targetNodeId);

		graph->CreateEdge(*sourceNode, *targetNode, isVisible);

		// Add connected nodes to the source adjacency list
		const_cast<Node*>(sourceNode)->AddAdjacentNode(*targetNode);

		edgeElement = edgeElement->NextSiblingElement("edge");
	}

	return true;
}

bool GraphParser::SaveGraph(const Graph& graph, const std::string& filename)
{
	tinyxml2::XMLDocument mXmlDocument; // TODO: rename

	XMLNode* root = mXmlDocument.NewElement("graphml");
	mXmlDocument.InsertFirstChild(root);

	// Note: The memory for an XMLElement is managed by the XMLDocument
	XMLElement* element = mXmlDocument.NewElement("graph");
	element->SetAttribute("id", "WorldGraph");
	element->SetAttribute("edgedefault", "directed");
	root->InsertEndChild(element);

	for (const auto& node : graph.Nodes())
	{
		XMLElement* nodeElement = mXmlDocument.NewElement("node");
		nodeElement->SetAttribute("id", std::to_string(node.Id()).c_str());
		nodeElement->SetAttribute("x", std::to_string(node.Position().x).c_str());
		nodeElement->SetAttribute("y", std::to_string(node.Position().y).c_str());
		element->InsertEndChild(nodeElement);
	}
	root->InsertEndChild(element);

	for (const auto& edge : graph.Edges())
	{
		XMLElement* edgeElement = mXmlDocument.NewElement("edge");
		edgeElement->SetAttribute("source", std::to_string(edge.SourceNode().Id()).c_str());
		edgeElement->SetAttribute("target", std::to_string(edge.TargetNode().Id()).c_str());
		edgeElement->SetAttribute("visible", "true");
		element->InsertEndChild(edgeElement);
	}
	root->InsertEndChild(element);

	if (mXmlDocument.SaveFile(filename.c_str()) != 0)
	{
		fprintf(stderr, "Error: The GraphParser failed to save %s.\n", filename.c_str());
		return false;
	}

	return true;
}