#include "graph_drawing_utils.h"

#include <vector>

#include "settings.h"


void DrawHermite(ImDrawList* drawList, ImVec2 p1, ImVec2 p2)
{
	int STEPS = 16;
	ImVec2 t1 = ImVec2(160.0f, 0.0f);
	ImVec2 t2 = ImVec2(160.0f, 0.0f);

	float dx = 1.f/STEPS;
	for (int step = 0; step <= STEPS; step++)
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


bool IsSlotHovered(Connection* c, ImVec2 offset)
{
	ImVec2 mousePos = ImGui::GetIO().MousePos;
	ImVec2 conPos = offset + c->pos;

	float xd = mousePos.x - conPos.x;
	float yd = mousePos.y - conPos.y;

	float distance = (xd * xd) + (yd * yd);
	float radius = Settings::SlotRadius;
	return distance < (radius * radius); 
}


void DrawNode(ImDrawList* drawList, ImVec2 offset, Node* node, int& node_selected)
{
	int node_hovered_in_scene = -1;
	bool open_context_menu = false;

	ImGui::PushID(node->id);

	// Display node contents first
	drawList->ChannelsSetCurrent(1); // Foreground
	bool old_any_active = ImGui::IsAnyItemActive();

	// Draw title in center
	ImVec2 textSize = ImGui::CalcTextSize(node->name);

	ImVec2 rectMin = offset + node->pos;
	ImVec2 rectMax = rectMin + node->size;
	
	ImVec2 pos = rectMin + Settings::NodeWindowPadding;
	pos.x = rectMin.x + (node->size.x / 2) - textSize.x / 2;
	ImGui::SetCursorScreenPos(pos);
	ImGui::Text("%s", node->name);

	// Save the size of what we have emitted and weither any of the widgets are being used
	bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
	
	// Display node box
	drawList->ChannelsSetCurrent(0); // Background

	ImGui::SetCursorScreenPos(rectMin);
	ImGui::InvisibleButton("node", node->size);

	if (ImGui::IsItemHovered())
	{
		node_hovered_in_scene = node->id;
		open_context_menu |= ImGui::IsMouseClicked(1);
	}

	bool node_moving_active = false;
	
	if (ImGui::IsItemActive() /*&& !s_dragNode.con*/)
		node_moving_active = true;

	ImU32 backgroundColor = Settings::NodeBgColor;

	if (node_hovered_in_scene == node->id) 
		backgroundColor = Settings::NodeBgHighlightColor;
	
	drawList->AddRectFilled(rectMin, rectMax, backgroundColor, 4.0f); 

	ImVec2 titleArea = rectMax;
	titleArea.y = rectMin.y + 30.0f;

	// Draw text bg area
	drawList->AddRectFilled(rectMin + ImVec2(1,1), titleArea, ImColor(40, 40, 40), 4.0f); 
	
	// Draw border of the node
	drawList->AddRect(rectMin, rectMax, Settings::NodeBorderColor, 4.0f); 

	offset = rectMin;

	offset.y += 40.0f;

	// Draw connection Slots
	for (Connection* slot : node->inputConnections)
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

	for (Connection* con : node->outputConnections)
	{
		textSize = ImGui::CalcTextSize(con->desc.name);

		ImGui::SetCursorScreenPos(offset + ImVec2(con->pos.x - (textSize.x + 10.0f), 0));

		ImColor conColor = Settings::SlotColor;

		// if (IsSlotHovered(con, rectMin))
		// 	conColor = ImColor(200, 200, 200);

		drawList->AddCircleFilled(rectMin + con->pos, Settings::SlotRadius, conColor); 

		offset.y += textSize.y + 2.0f;
	}


	//for (int i = 0; i < node->outputConnections.size(); ++i)
	//	drawList->AddCircleFilled(offset + node->outputSlotPos(i), NODE_SLOT_RADIUS, ImColor(150,150,150,150));

	if (node_widgets_active || node_moving_active)
		node_selected = node->id;

	// Drag node 
	if (node_moving_active && ImGui::IsMouseDragging(0))
		node->pos = node->pos + ImGui::GetIO().MouseDelta;

	ImGui::PopID();
}
