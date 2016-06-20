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
	Slot();
	Slot(ImVec2 pPos, bool pIsOutput = false);
	ImVec2 GetPos();
	ImVec2 GetWorldPos();
	void SetPos(ImVec2 pPos);
	const bool IsOutput();
public:
	Node* parent;
	ImVec2 pos;
	DataType dataType;
private:
	bool _isOutput = false;
};

struct NodeType
{
	const char* name;
	DataType inputSlots[MAX_INPUT_COUNT];
	DataType outputSlot;
	unsigned numInputs;
};


extern struct NodeType gNodeTypes[5];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Node {
public:
	Graph* parent;
	ImVec2 pos;

	const char * name;
	std::vector<Slot*> inputs;
	Slot* output;

	unsigned fboID = 0; // OpenGL blablabla
	unsigned programID = 0;
	unsigned texID = 0;
	unsigned inputTexID = 0;

	Node(ImVec2 pPos, NodeType pType, unsigned temp);
	const unsigned GetID();
	ImVec2 GetPos();
	ImVec2 GetWorldPos();
	void ComputeOutput();

private:
	unsigned _id;

	void SetupSlots(NodeType pType);
	
	
};

#endif // NODE_H