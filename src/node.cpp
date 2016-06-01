#include "node.h"

struct NodeType gNodeTypes[] =
{
	{
		"Blend",
		// Input connections
		{
			DataType::Color,
			DataType::Color,
		},
		// Output
		DataType::Color,
	},

	{
		"Threshold",
		// Input connections
		{
			DataType::Color,
		},
		// Output
		DataType::Color,
		
	},
};



Node::Node(ImVec2 pPos) {
	pos = pPos;
}