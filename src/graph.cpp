#include "graph.h"

void Graph::AddNode(ImVec2 pPos, NodeType pType) {
	Node* node = new Node(pPos, pType);
	node->name = pType.name;

	_nodes.push_back(node);
}

const unsigned Graph::GetNumNodes() {
	return _nodes.size();
}

const std::vector<Node*> & Graph::GetNodeData() {
	return _nodes;
}