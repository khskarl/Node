#include "graph.h"

void Graph::AddNode(ImVec2 pPos, NodeType pType) {
	Node* node = new Node(pPos, pType);
	node->name = pType.name;

	_nodes.push_back(node);
}

void Graph::AddLink(Slot* fromSlot, Slot* toSlot) {
	Link* link = new Link(fromSlot, toSlot);
	_links.push_back(link);
}

const unsigned Graph::GetNumNodes() {
	return _nodes.size();
}

const std::vector<Node*>& Graph::GetNodeData() {
	return _nodes;
}

const std::vector<Link*>& Graph::GetLinkData() {
	return _links;
}