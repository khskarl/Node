#ifndef NODE_H
#define NODE_H

#include <vector>
#include <imgui.h>
#include "settings.h"

class Node;
class Graph;

enum DataType 
{
	Color,
};

class Slot {
public:
	ImVec2 GetPos();
	ImVec2 GetWorldPos();
	void SetPos(ImVec2 pPos);
public:
	Node* parent;
	ImVec2 pos;
	DataType dataType;
};

struct NodeType
{
	const char* name;
	DataType inputSlots[MAX_INPUT_COUNT];
	DataType outputSlot;
	unsigned numInputs;
};


extern struct NodeType gNodeTypes[3];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Node {
public:
	Graph* parent;
	ImVec2 pos;

	const char * name;
	std::vector<Slot*> inputs;
	Slot* output;

	Node(ImVec2 pPos, NodeType pType);
	const unsigned GetID();
	ImVec2 GetPos();
	ImVec2 GetWorldPos();

private:
	unsigned _id;

	void SetupSlots(NodeType pType);
	
};

#endif // NODE_H