#include "graph.h"
#include <iostream>

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
	ComputeChildren(toNode);
}

void Graph::RemoveNode(Node * target) {
	std::cout << "A\n";
	for(auto it = _links.begin(); it != _links.end(); it++) {
		Node* fromNode = (*it)->from->parent;
		Node* toNode = (*it)->to->parent;

		if (target == fromNode || target == toNode)
			_links.erase(it);
	}
	std::cout << "C\n";
	for(auto it = _nodes.begin(); it != _nodes.end(); it++) {
		if (target == *it)
			_nodes.erase(it);
	}

	std::cout << "D\n";


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

void Graph::ComputeChildren(Node * target) {
	target->ComputeOutput();

	for(Link* link : _links) {
		Node* fromNode = link->from->parent;
		if (target == fromNode) {
			Node* toNode = link->to->parent;
			ComputeChildren(toNode);
		}	
	}
}