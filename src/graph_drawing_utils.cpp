#include "graph_drawing_utils.h"

#include <vector>

#include "graph.h"
#include "settings.h"


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

void DrawSlots(ImDrawList* drawList, ImVec2 offset, Node* node)
{

}

// Shouldn't be here
bool IsSlotHovered(Slot* slot, ImVec2 offset)
{
	ImVec2 mousePos = ImGui::GetIO().MousePos;
	ImVec2 slotPos = offset + slot->pos;

	float xd = mousePos.x - slotPos.x;
	float yd = mousePos.y - slotPos.y;

	float distance = (xd * xd) + (yd * yd);
	float radius = Settings::SlotRadius;
	return distance < (radius * radius); 
}


void DrawNode(ImDrawList* drawList, ImVec2 offset, Node* node, int& node_selected)
{
	int hoveredNodeID = -1;

	ImGui::PushID(node->GetID());

	// Display node contents first
	bool old_any_active = ImGui::IsAnyItemActive();

	// Compute useful size and positions
	ImVec2 textSize = ImGui::CalcTextSize(node->name);

	ImVec2 rectMin = offset + node->pos;
	ImVec2 rectMax = rectMin + Settings::NodeSize;
		
	// Save the size of what we have emitted and weither any of the widgets are being used
	bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
	
	// Display node box
	ImGui::SetCursorScreenPos(rectMin);
	ImGui::InvisibleButton("node", Settings::NodeSize);

	if (ImGui::IsItemHovered())
		hoveredNodeID = node->GetID();
	
	bool node_moving_active = false;
	
	if (ImGui::IsItemActive() /*&& !s_dragNode.con*/)
		node_moving_active = true;

	ImU32 backgroundColor = Settings::NodeBgColor;

	if (hoveredNodeID == node->GetID()) 
		backgroundColor = Settings::NodeBgHighlightColor;
	

	ImVec2 titleArea = rectMax;
	titleArea.y = rectMin.y + 25.0f;

	// Draw border of the node
	{	
		ImVec2 borderRectMin = rectMin - ImVec2(4, 4);
		ImVec2 borderRectMax = rectMax + ImVec2(4, 4);
		
		drawList->AddRectFilled(borderRectMin, 
						  		borderRectMax, 
						  		Settings::NodeBorderColor, 
						  		4.0f); 
		// Draw NodeBackground
		drawList->AddRectFilled(rectMin, rectMax, backgroundColor, 4.0f); 
	}


	// Draw title background
	drawList->AddRectFilled(rectMin + ImVec2(1,1), titleArea, ImColor(40, 40, 40), 4.0f); 
	
	// Draw title
	{
		ImVec2 pos = rectMin + Settings::NodeWindowPadding;
		pos.x = rectMin.x + (Settings::NodeSize.x / 2) - textSize.x / 2;
		ImGui::SetCursorScreenPos(pos);
		ImGui::Text("%s", node->name);
	}

	offset = rectMin;
	offset.y += 40.0f;

	// Draw input slots
	for (Slot* slot : node->inputs)
	{
		ImGui::SetCursorScreenPos(offset + ImVec2(10.0f, 0));

		ImColor slotColor = Settings::NodeColor;

		// if (IsSlotHovered(slot, rectMin))
		// 	slotColor = ImColor(200, 200, 200);

		drawList->AddCircleFilled(rectMin + slot->pos, 
								  Settings::SlotRadius, 
								  slotColor); 

		offset.y += textSize.y + 2.0f;
	}

	offset = rectMin;
	offset.y += 40.0f;

	// Output rendering
	{
		Slot * slot = node->output;

		ImColor slotColor = Settings::SlotColor;
		// if (IsSlotHovered(con, rectMin))
		// 	slotColor = ImColor(200, 200, 200);

		float slotRadius = Settings::SlotRadius;

		drawList->AddCircleFilled(rectMin + slot->pos, 
								  slotRadius, 
								  slotColor); 

		offset.y += textSize.y + 2.0f;
	}


	//for (int i = 0; i < node->outputLinks.size(); ++i)
	//	drawList->AddCircleFilled(offset + node->outputSlotPos(i), NODE_SLOT_RADIUS, ImColor(150,150,150,150));

	if (node_widgets_active || node_moving_active)
		node_selected = node->GetID();

	// Drag node 
	if (node_moving_active && ImGui::IsMouseDragging(0))
		node->pos = node->pos + ImGui::GetIO().MouseDelta;

	ImGui::PopID();
}
