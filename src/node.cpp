#include "node.h"
#include "settings.h"
#include "opengl_utils.h"

struct NodeType gNodeTypes[] =
{
	{
		"Circle",
		// Input connections
		{			
		},
		// Output
		DataType::Color,
		0,
	},
	{
		"UV",
		// Input connections
		{			
		},
		// Output
		DataType::Color,
		0,
	},
	{
		"Vertical Waves",
		// Input connections
		{
			DataType::Color,
		},
		// Output
		DataType::Color,
		1,
	},
	{
		"Horizontal Waves",
		// Input connections
		{
			DataType::Color,
		},
		// Output
		DataType::Color,
		1,
	},

	{
		"Threshold",
		// Input connections
		{
			DataType::Color,
		},
		// Output
		DataType::Color,
		1,
	},
};

// SLOT
Slot::Slot(){

}

Slot::Slot(ImVec2 pPos, bool pIsOutput){
	pos = pPos;
	_isOutput = pIsOutput;
}

ImVec2 Slot::GetPos(){
	return pos;
}

ImVec2 Slot::GetWorldPos(){
	return pos + parent->GetWorldPos();
}

void Slot::SetPos(ImVec2 pPos){
	pos = pPos;
}

const bool Slot::IsOutput() {
	return _isOutput;
}

// NODE
static const unsigned GetUniqueNodeID() {
	static unsigned uid = 0;
	return uid++;
}

void Node::ComputeOutput() {
	if (inputs.size() > 0)
		DrawFramebuffer(fboID, programID, inputTexID);
	else
		DrawFramebuffer(fboID, programID);
}

void Node::SetupSlots(NodeType pType) {
	// Create slots
	for (unsigned i = 0; i < pType.numInputs; ++i)
	{
		Slot * slot = new Slot;
		slot->dataType = pType.inputSlots[i];
		inputs.push_back(slot);
	}

	// Set input slots positions
	{
		float slotHeight = 0;
		float dx = Settings::NodeSize.y / inputs.size();

		for (Slot * slot : inputs)
		{
			slot->parent = this;
			slot->pos = ImVec2(0.0f, slotHeight + dx / 2.f);
			slotHeight += dx;
		}
	}

	// Setup output slot
	{
		ImVec2 pos = ImVec2(Settings::NodeSize.y,
							Settings::NodeSize.y / 2.f);

		Slot * slot = new Slot(pos, true);
		slot->parent = this;
		slot->dataType = pType.outputSlot;

		output = slot;
	}


}

Node::Node(ImVec2 pPos, NodeType pType, unsigned temp) {
	_id = GetUniqueNodeID();
	pos = pPos;
	name = pType.name;
	
	this->SetupSlots(pType);
	fboID = CreateFramebuffer(texID);
	programID = temp;
	ComputeOutput();
}

const unsigned Node::GetID() {
	return _id;
}

ImVec2 Node::GetPos(){
	return pos;
}

ImVec2 Node::GetWorldPos(){
	return pos;
}