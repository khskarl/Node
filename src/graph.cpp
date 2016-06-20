#include "graph.h"

void Graph::AddNode(ImVec2 pPos, NodeType pType, unsigned temp) {
	Node* node = new Node(pPos, pType, temp);
	node->name = pType.name;

	_nodes.push_back(node);
}

void Graph::AddLink(Slot* fromSlot, Slot* toSlot) {
	Link* link = new Link(fromSlot, toSlot);
	_links.push_back(link);

	Node* fromNode = fromSlot->parent;
	Node* toNode = toSlot->parent;

	toNode->inputTexID = fromNode->texID;
	toNode->ComputeOutput();
}

const unsigned Graph::GetNumNodes() {
	return _nodes.size();
}

std::vector<Node*>& Graph::GetNodeData() {
	return _nodes;
}

std::vector<Link*>& Graph::GetLinkData() {
	return _links;
}

void Graph::ComputeHierarchy(Node * target) {
	target->ComputeOutput();
}