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

struct Link
{
	ImVec2 pos;
	SlotDesc desc;

	inline Link()
	{
		pos.x = pos.y = 0.0f;
		input = 0;
	}

	float color[4];

	Link* input;
	std::vector<Link*> output;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Node types

struct NodeType
{
	const char* name;
	SlotDesc inputLinks[MAX_CONNECTION_COUNT];
	SlotDesc outputLinks[MAX_CONNECTION_COUNT];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct NodeType s_nodeTypes[] =
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
	std::vector<Link*> inputLinks;
	std::vector<Link*> outputLinks;
};

#endif // GRAPH_H