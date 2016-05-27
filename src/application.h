#ifndef NODE_PROGRAM_H
#define NODE_PROGRAM_H

#include <vector>
#include <stdint.h>

#include <imgui.h>
#include "imgui_dock.h"


#define sizeofArray(t) (sizeof(t) / sizeof(t[0]))
#define MAX_CONNECTION_COUNT 4

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x+rhs.x, lhs.y+rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x-rhs.x, lhs.y-rhs.y); }


enum SlotType 
{
	Color,
	Vec3,
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

	float v3[3];

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

class Application {
private:
	bool _isGraphEditorOpen;
	void ShowGraphEditor();
public:
	void Show();
};

#endif // NODE_PROGRAM_H