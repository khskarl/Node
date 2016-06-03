#ifndef NODE_H
#define NODE_H

#include <vector>
#include <imgui.h>
#include "settings.h"

class Node;

enum DataType 
{
	Color,
};

struct Slot {
	Node* parent;
	ImVec2 pos;
	DataType dataType;
};

struct NodeType
{
	const char* name;
	DataType inputSlots[MAX_INPUT_COUNT];
	DataType outputSlot;
};


extern struct NodeType gNodeTypes[3];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Node {
public:
	ImVec2 pos;

	const char * name;
	std::vector<Slot*> inputs;
	Slot* output;

	Node(ImVec2 pPos, NodeType pType);
	const unsigned GetID();

private:
	unsigned _id;

	void SetupSlots(NodeType pType);
	
};

#endif // NODE_H