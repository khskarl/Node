#include "graph.h"

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