#ifndef NODE_H
#define NODE_H

#include <vector>
#include <imgui.h>
#include "settings.h"


enum DataType 
{
	Color,
};

struct Slot {
	ImVec2 pos;
	DataType dataType;
};

struct NodeType
{
	const char* name;
	DataType inputSlots[MAX_INPUT_COUNT];
	DataType outputSlot;
};


extern struct NodeType gNodeTypes[2];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Node {
public:
	Node(ImVec2 pPos);
	ImVec2 pos;

	int id;
	const char * name;
	std::vector<Slot*> inputs;
	Slot* output;

private:
	
};

#endif // NODE_H