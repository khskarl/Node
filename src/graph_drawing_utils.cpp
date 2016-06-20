#include "graph_drawing_utils.h"

#include <vector>

#include "graph.h"
#include "settings.h"

static ImVec2 _graphOffset = ImVec2(0, 0);
static Node* _hoveredNode = nullptr;
static Slot* _hoveredSlot = nullptr;

void SetDrawingOffset(ImVec2 offset) {
	_graphOffset = offset;
}

void SetHoveredNode(Node* pHoveredNode) {
	_hoveredNode = pHoveredNode;
}

void SetHoveredSlot(Slot* pHoveredSlot) {
	_hoveredSlot = pHoveredSlot;
}

void DrawHermite(ImDrawList* drawList, ImVec2 p1, ImVec2 p2)
{
	int STEPS = 20;
	ImVec2 t1 = ImVec2(160.0f, 0.0f);
	ImVec2 t2 = ImVec2(160.0f, 0.0f);

	float dx = 1.f/STEPS;
	for (int step = 0; step <= STEPS; ++step)
	{
		float t = dx * step;
		float h1 = +2*t*t*t - 3*t*t + 1.0f;
		float h2 = -2*t*t*t + 3*t*t;
		float h3 =    t*t*t - 2*t*t + t;
		float h4 =    t*t*t -   t*t;

		float x = h1 * p1.x + h2 * p2.x + h3 * t1.x + h4 * t2.x;
		float y = h1 * p1.y + h2 * p2.y + h3 * t1.y + h4 * t2.y;
		
		drawList->PathLineTo(ImVec2(x, y));
	}

	drawList->PathStroke(Settings::LinkColor, false, 3.0f);
}

void DrawLinks(ImDrawList* drawList, Graph& graph)
{
	std::vector<Link*> links = graph.GetLinkData();

	for (Link* link : links)
	{
		Slot* fromSlot = link->from;
		Slot* toSlot   = link->to;

		ImVec2 fromPos = fromSlot->GetWorldPos() + _graphOffset;
		ImVec2 toPos = toSlot->GetWorldPos() + _graphOffset;

		DrawHermite(drawList, 
					fromPos, 
					toPos);
	}
}


// Shouldn't be here
bool IsSlotHovered(Slot* slot)
{
	ImVec2 mousePos = ImGui::GetIO().MousePos;
	ImVec2 slotPos = slot->GetWorldPos() + _graphOffset;

	float xd = mousePos.x - slotPos.x;
	float yd = mousePos.y - slotPos.y;

	float distance = (xd * xd) + (yd * yd);
	float radius = Settings::SlotRadius;

	return distance < (radius * radius); 
}

static void DrawSlot(ImDrawList* drawList, ImVec2 parentPos, Slot* slot) {
	ImColor slotColor = Settings::NodeColor;

	if (IsSlotHovered(slot))
		slotColor = ImColor(200, 200, 200);

	drawList->AddCircleFilled(parentPos + slot->pos, 
							  Settings::SlotRadius, 
							  slotColor); 
}

static void DrawSlots(ImDrawList* drawList, ImVec2 parentPos, Node* node) {
	// Draw input slots
	for (Slot* slot : node->inputs)
	{
		DrawSlot(drawList, parentPos, slot);
	}

	// Draw output slot
	{
		DrawSlot(drawList, parentPos, node->output);; 
	}
}

Node* GetHoveredNode(Graph & graph) {
	Node* hoveredNode = nullptr;
	for (Node* node : graph.GetNodeData())
	{
	 	ImGui::PushID(node->GetID());
		
		// Compute dimensions of the node
		ImVec2 rectMin = _graphOffset + node->pos;
		// ImVec2 rectMax = rectMin + Settings::NodeSize;

		// Display node box
		ImGui::SetCursorScreenPos(rectMin);
		ImGui::InvisibleButton("node", Settings::NodeSize);

		if (ImGui::IsItemHovered())
			hoveredNode = node;

		ImGui::PopID();
	}
	return hoveredNode;
}

void DragNode(Node* node) {
	node->pos = node->pos + ImGui::GetIO().MouseDelta;
}

Slot* GetHoveredSlot(Graph & graph) {
	Slot* hoveredSlot = nullptr;

	for (Node* node : graph.GetNodeData())
	{
		for (Slot* slot : node->inputs)
		{
			if(IsSlotHovered(slot))
				hoveredSlot = slot;
		}

		Slot* slot = node->output;
		{
			if(IsSlotHovered(slot))
				hoveredSlot = slot;
		}
	}
	
	return hoveredSlot;
}


void DrawNodes(ImDrawList* drawList, Graph & graph) 
{
	for (Node * node : graph.GetNodeData())
		DrawNode(drawList, node);
}

void DrawNode(ImDrawList* drawList, Node* node)
{
 	ImGui::PushID(node->GetID());
	
	// Compute dimensions of the node
	ImVec2 rectMin = _graphOffset + node->pos;
	ImVec2 rectMax = rectMin + Settings::NodeSize;

	// Display node box
	ImGui::SetCursorScreenPos(rectMin);

	bool isNodeHovered = false;

	if (_hoveredNode == node) 
		isNodeHovered = true;

	ImU32 backgroundColor = (Settings::NodeBgColor); 
	
	// Draw border of the node
	{	
		float borderSize = 4;
		ImVec2 borderRectMin = rectMin - ImVec2(borderSize, borderSize);
		ImVec2 borderRectMax = rectMax + ImVec2(borderSize, borderSize);
		
		ImU32 borderColor = Settings::NodeBorderColor;
		if (isNodeHovered == true)
			borderColor = Settings::NodeBorderHighlightColor;

		drawList->AddRectFilled(borderRectMin, 
						  		borderRectMax, 
						  		borderColor, 
						  		4.0f); 
		// Draw NodeBackground
		drawList->AddRectFilled(rectMin, rectMax, backgroundColor, 4.0f);
		drawList->AddImage((void*)node->fboID, rectMin, rectMax); 
	}

	// Draw title
	{
		ImVec2 textSize = ImGui::CalcTextSize(node->name);
		ImVec2 titleArea = rectMax;
		titleArea.y = rectMin.y + 25.0f;

		// Draw title background
		drawList->AddRectFilled(rectMin + ImVec2(1,1), titleArea, ImColor(40, 40, 40, 100), 4.0f); 

		ImVec2 pos = rectMin + Settings::NodeWindowPadding;
		pos.x = rectMin.x + (Settings::NodeSize.x / 2) - textSize.x / 2;

		ImGui::SetCursorScreenPos(pos);
		ImGui::Text("%s", node->name);
		ImGui::Text("%u", node->texID);
		ImGui::Text("%u", node->inputTexID);
	}

	DrawSlots(drawList, rectMin, node);

	ImGui::PopID();
}
