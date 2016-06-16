#include "graph_drawing_utils.h"

#include <vector>

#include "graph.h"
#include "settings.h"

static ImVec2 _graphOffset = ImVec2(0, 0);
static Node* _selectedNode = nullptr;

void SetDrawingOffset(ImVec2 offset) {
	_graphOffset = offset;
}

void SetSelectedNode(Node* pSelectedNode) {
	_selectedNode = pSelectedNode;
}

void DrawHermite(ImDrawList* drawList, ImVec2 p1, ImVec2 p2)
{
	int STEPS = 16;
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

		// Display node contents first
		// bool old_any_active = ImGui::IsAnyItemActive();

		// Save the size of what we have emitted and weither any of the widgets are being used
		// bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
		
		// Compute dimensions of the node
		ImVec2 rectMin = _graphOffset + node->pos;
		ImVec2 rectMax = rectMin + Settings::NodeSize;

		// Display node box
		ImGui::SetCursorScreenPos(rectMin);
		ImGui::InvisibleButton("node", Settings::NodeSize);

		if (ImGui::IsItemHovered())
			hoveredNode = node;


		// if (_selectedNode == node) 
		// 	isNodeHovered = true;
		
		// bool node_moving_active = false;
		
		// if (ImGui::IsItemActive() /*&& !s_dragNode.con*/)
		// 	node_moving_active = true;

		// 	if (node_widgets_active || node_moving_active)
		// 	_selectedNode = node;

		// // Drag node 
		// if (node_moving_active && ImGui::IsMouseDragging(0))
		// 	node->pos = node->pos + ImGui::GetIO().MouseDelta;

		ImGui::PopID();
	}
	return hoveredNode;
}

Slot* GetHoveredSlot(Graph & graph) {
	Slot* hoveredSlot = nullptr;

	for (Node* node : graph.GetNodeData())
	{
		for (Slot* slot : node->inputs)
		{
			// DrawSlot(drawList, parentPos, slot);
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

	if (_selectedNode == node) 
		isNodeHovered = true;

	ImU32 backgroundColor = Settings::NodeBgColor; 
	
	// Draw border of the node
	{	
		ImVec2 borderRectMin = rectMin - ImVec2(4, 4);
		ImVec2 borderRectMax = rectMax + ImVec2(4, 4);
		
		ImU32 borderColor = Settings::NodeBorderColor;
		if (isNodeHovered == true)
			borderColor = Settings::NodeBorderHighlightColor;

		drawList->AddRectFilled(borderRectMin, 
						  		borderRectMax, 
						  		borderColor, 
						  		4.0f); 
		// Draw NodeBackground
		drawList->AddRectFilled(rectMin, rectMax, backgroundColor, 4.0f); 
	}

	// Draw title
	{
		ImVec2 textSize = ImGui::CalcTextSize(node->name);
		ImVec2 titleArea = rectMax;
		titleArea.y = rectMin.y + 25.0f;

		// Draw title background
		drawList->AddRectFilled(rectMin + ImVec2(1,1), titleArea, ImColor(40, 40, 40), 4.0f); 

		ImVec2 pos = rectMin + Settings::NodeWindowPadding;
		pos.x = rectMin.x + (Settings::NodeSize.x / 2) - textSize.x / 2;

		ImGui::SetCursorScreenPos(pos);
		ImGui::Text("%s", node->name);
	}

	DrawSlots(drawList, rectMin, node);

	ImGui::PopID();
}
