#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <imgui.h>

#include "node.h"

#define sizeofArray(t) (sizeof(t) / sizeof(t[0]))

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Link {
	Link (Slot* pFrom, Slot* pTo) {
		from = pFrom;
		to = pTo;
	}
	Slot* from;
	Slot* to;
};


class Graph {
public:
	void AddNode(ImVec2 pPos, NodeType pType);
	void AddLink(Slot* fromSlot, Slot* toSlot);

	const std::vector<Node*>& GetNodeData();
	const std::vector<Link*>& GetLinkData();
	const unsigned GetNumNodes();
	const unsigned GetNumLinks();

private:
	std::vector<Node*> _nodes;
	std::vector<Link*> _links;
};


#endif // GRAPH_H