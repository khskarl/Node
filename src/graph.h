#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

#define sizeofArray(t) (sizeof(t) / sizeof(t[0]))
#define MAX_CONNECTION_COUNT 4

enum SlotType 
{
	Color,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct SlotDesc
{
	const char* name;
	SlotType type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Connection
{
	ImVec2 pos;
	SlotDesc desc;

	inline Connection()
	{
		pos.x = pos.y = 0.0f;
		input = 0;
	}

	float color[4];

	struct Connection* input;
	std::vector<Connection*> output;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Node types

struct NodeType
{
	const char* name;
	SlotDesc inputConnections[MAX_CONNECTION_COUNT];
	SlotDesc outputConnections[MAX_CONNECTION_COUNT];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static struct NodeType s_nodeTypes[] =
{
	{
		"Blend",
		// Input connections
		{
			{ "Input1", SlotType::Color },
			{ "Input2", SlotType::Color },
		},
		// Output
		{
			{ "Out", SlotType::Color },
		},
	},

	{
		"Threshold",
		// Input connections
		{
			{ "Input1", SlotType::Color },
		},
		// Output
		{
			{ "Out", SlotType::Color },
		},
	},
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Node
{
	ImVec2 pos;
	ImVec2 size;
	int id;
	const char* name;
	std::vector<Connection*> inputConnections;
	std::vector<Connection*> outputConnections;
};

#endif // GRAPH_H