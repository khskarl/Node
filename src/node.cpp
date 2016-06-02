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

	{
		"2D Transform",
		// Input connections
		{
			DataType::Color,
		},
		// Output
		DataType::Color,
		
	},
};


static const unsigned GetUniqueNodeID() {
	static unsigned uid = 0;
	return uid++;
}

void Node::SetupSlots(NodeType pType) {
	// for (int i = 0; i < (int)sizeofArray(gNodeTypes); ++i)
	// {
	// }
	// Create slots
	for (DataType slotDataType : pType.inputSlots)
	{		
		// if (!slotDataType)
			// continue;
		Slot * slot = new Slot;
		slot->pos = ImVec2(0, 0);
		slot->dataType = slotDataType;
		
		inputs.push_back(slot);
	}

	// Set slots positions
	{
		float slotHeight = 0;
		float dx = Settings::NodeSize.y / inputs.size();

		for (Slot * slot : inputs)
		{
			slot->pos = ImVec2(0.0f, slotHeight + dx / 2.f);
			slotHeight += dx;
		}
	}
}

Node::Node(ImVec2 pPos, NodeType pType) {
	pos = pPos;
	_id = GetUniqueNodeID();
	name = pType.name;
	
	this->SetupSlots(pType);

	

	// ImVec2 size;
	// size.x = Settings::NodeSize.x;	
	// size.y = Settings::NodeSize.y / 2.f;

	// set the positions for the output nodes
	// for (Slot* l : node->outputLinks)
	// {		
	// 	l->pos = ImVec2(size.x, size.y);
	// }

}

const unsigned Node::GetID() {
	return _id;
}