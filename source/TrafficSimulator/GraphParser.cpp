#include "pch.h"
//#include "GraphParser.h"
//
//GraphParser::GraphParser(Graph* graph)
//	: mGraphHandle(graph)
//{
//}
//
//Graph* GraphParser::ConstructGraphFromFile(const std::string& filename)
//{
//	// First clear the graph 
//	// TODO: clear the existing graph, it may already have data stored
//	//mGraphHandle.Clear(); // TODO: implement a function in graph to free all memory
//
//	tinyxml2::XMLDocument doc;
//	XMLError loadStatus = doc.LoadFile(filename.c_str());
//	if (loadStatus == XML_ERROR_FILE_COULD_NOT_BE_OPENED || loadStatus == XML_ERROR_FILE_NOT_FOUND)
//	{
//		fprintf(stderr, "Error: The GraphParser failed to load %s.\n", filename.c_str());
//	}
//
//	//tinyxml2::XMLPrinter printer(doc.);
//	return nullptr;
//}
