#include "graph_drawing_utils.h"

#include <vector>

const float NODE_SLOT_RADIUS = 6.0f;
const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);
const float SLOTS_SPACING = 4.0f;

void DrawHermite(ImDrawList* drawList, ImVec2 p1, ImVec2 p2)
{
	int STEPS = 12;
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
		drawList->PathLineTo(ImVec2(h1 * p1.x + h2 * p2.x + h3 * t1.x + h4 * t2.x, 
									h1 * p1.y + h2 * p2.y + h3 * t1.y + h4 * t2.y));
	}

	drawList->PathStroke(ImColor(200,200,100), false, 3.0f);
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

	ImVec2 node_rect_min = offset + node->pos;
	ImVec2 node_rect_max = node_rect_min + node->size;
	
	ImVec2 pos = node_rect_min + NODE_WINDOW_PADDING;
	pos.x = node_rect_min.x + (node->size.x / 2) - textSize.x / 2;
	ImGui::SetCursorScreenPos(pos);
	ImGui::Text("%s", node->name);

	// Save the size of what we have emitted and weither any of the widgets are being used
	bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
	
	// Display node box
	drawList->ChannelsSetCurrent(0); // Background

	ImGui::SetCursorScreenPos(node_rect_min);
	ImGui::InvisibleButton("node", node->size);

	if (ImGui::IsItemHovered())
	{
		node_hovered_in_scene = node->id;
		open_context_menu |= ImGui::IsMouseClicked(1);
	}

	bool node_moving_active = false;
	
	if (ImGui::IsItemActive() /*&& !s_dragNode.con*/)
		node_moving_active = true;

	ImU32 node_bg_color = node_hovered_in_scene == node->id ? ImColor(75,75,75) : ImColor(60,60,60);
	drawList->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f); 

	ImVec2 titleArea = node_rect_max;
	titleArea.y = node_rect_min.y + 30.0f;

	// Draw text bg area
	drawList->AddRectFilled(node_rect_min + ImVec2(1,1), titleArea, ImColor(100,0,0), 4.0f); 
	drawList->AddRect(node_rect_min, node_rect_max, ImColor(100,100,100), 4.0f); 

	offset = node_rect_min;

	offset.y += 40.0f;

	// Draw connection Slots
	for (Connection* slot : node->inputConnections)
	{
		ImGui::SetCursorScreenPos(offset + ImVec2(10.0f, 0));

		ImColor slotColor = ImColor(150, 150, 150);

		// if (IsSlotHovered(slot, node_rect_min))
		// 	slotColor = ImColor(200, 200, 200);

		drawList->AddCircleFilled(node_rect_min + slot->pos, NODE_SLOT_RADIUS, slotColor); 

		offset.y += textSize.y + 2.0f;
	}

	offset = node_rect_min;
	offset.y += 40.0f;

	for (Connection* con : node->outputConnections)
	{
		textSize = ImGui::CalcTextSize(con->desc.name);

		ImGui::SetCursorScreenPos(offset + ImVec2(con->pos.x - (textSize.x + 10.0f), 0));

		ImColor conColor = ImColor(150, 150, 150);

		// if (IsSlotHovered(con, node_rect_min))
		// 	conColor = ImColor(200, 200, 200);

		drawList->AddCircleFilled(node_rect_min + con->pos, NODE_SLOT_RADIUS, conColor); 

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
