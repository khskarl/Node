#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <imgui.h>

#include "node.h"

#define sizeofArray(t) (sizeof(t) / sizeof(t[0]))

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Link
{
	Node * input;
	std::vector<Link*> output;
};


class Graph {
public:
	void AddNode(ImVec2 pPos, NodeType *pType);
	void AddLink();

	const std::vector<Node*> & GetNodeData();
private:
	std::vector<Node*> _nodes;
	std::vector<Link> _links;
};


#endif // GRAPH_H