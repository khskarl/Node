#include "application.h"

#include <iostream>

#include "settings.h"
#include "graph_drawing_utils.h"

#include "graph.h"

using std::cout;

Application::Application() {

}

void Application::Init() {
	_graph.AddNode(ImVec2(100, 200), gNodeTypes[0]);
	_graph.AddNode(ImVec2(600, 200), gNodeTypes[1]);
	_graph.AddNode(ImVec2(300, 300), gNodeTypes[2]);
	Slot* from = _graph.GetNodeData()[0]->output;
	Slot* to   = _graph.GetNodeData()[1]->inputs[0];

	_graph.AddLink(from, to);
}

void Application::UpdateGraphInteraction(ImDrawList* drawList) {
	
	Slot* hoveredSlot = GetHoveredSlot(_graph);
	SetHoveredSlot(hoveredSlot);
	Node* hoveredNode = GetHoveredNode(_graph); 
	SetHoveredNode(hoveredNode);


	static Slot* otherSlot = nullptr;

	switch(_linkDragState) {
	case LinkDragState::Idle:
		if (hoveredSlot != nullptr)
			_linkDragState = LinkDragState::HoveringSlot;

		ImGui::Text("A");
		break;

	case LinkDragState::HoveringSlot:
		if (hoveredSlot == nullptr) {
			_linkDragState = LinkDragState::Idle;
			break;
		}

		if (ImGui::IsMouseClicked(0) == true) 
		{
			otherSlot = hoveredSlot;
			if (otherSlot->IsOutput())
				_linkDragState = LinkDragState::DragingInputLink;
			else					
				_linkDragState = LinkDragState::DragingOutputLink;
		}
	
		ImGui::Text("B");
		break;

	case LinkDragState::DragingInputLink:
		if (ImGui::IsMouseReleased(0)) {
			if (hoveredSlot != nullptr) {
				_graph.AddLink(otherSlot, hoveredSlot);
			}

			_linkDragState = LinkDragState::Idle;
			break;
		}

		DrawHermite(drawList, 
					otherSlot->GetWorldPos(), 
					ImGui::GetIO().MousePos);
		
		ImGui::Text("C");
		break;

	case LinkDragState::DragingOutputLink:
		if (ImGui::IsMouseReleased(0)) { 
			if (hoveredSlot != nullptr) {
				_graph.AddLink(hoveredSlot, otherSlot);
			}

			_linkDragState = LinkDragState::Idle;
			break;
		}
		
		DrawHermite(drawList,  
					ImGui::GetIO().MousePos,
					otherSlot->GetWorldPos());
		
		ImGui::Text("D");
		break;

	}

	
	if (ImGui::IsMouseDown(0) && hoveredNode && hoveredSlot == nullptr) {
		DragNode(hoveredNode);
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Application::ShowGraphEditor()
{
	ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Graph Editor", &_isGraphEditorOpen);

	// TODO: Remove/refactor those
	int node_hovered_in_list = -1;
	int node_hovered_in_scene = -1;

	// End TODO

	ImGui::SameLine();
	ImGui::BeginGroup();

	// Create our child canvas
	// ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", graphOffset.x, graphOffset.y);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, Settings::BackgroundColor);
	ImGui::BeginChild("scroll_region", 
					  ImVec2(0,0), 
					  true, 
					  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
	ImGui::PushItemWidth(120.0f);

	////////////////////
	
	// updateDraging(graphOffset);

	// Gotta set this to fix the position inside "graph_drawing_utils"
	// so it will take into consideration the offset
	SetDrawingOffset(graphOffset); 

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	UpdateGraphInteraction(drawList);
	
	DrawLinks(drawList, _graph);
	DrawNodes(drawList, _graph);

	////////////////////

	// Open context menu
	bool open_context_menu = false;
	if (ImGui::IsAnyItemHovered() && 
		ImGui::IsMouseHoveringWindow() && 
		ImGui::IsMouseClicked(1))
	{
		node_hovered_in_list = node_hovered_in_scene = -1;
		open_context_menu = true;
	}

	if (open_context_menu)
	{
		ImGui::OpenPopup("context_menu");
		// if (node_hovered_in_list != -1)
		// 	node_selected = node_hovered_in_list;
		// if (node_hovered_in_scene != -1)
		// 	node_selected = node_hovered_in_scene;
	}

	// Draw context menu
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8,8));
		if (ImGui::BeginPopup("context_menu"))
		{	
			for (int i = 0; i < (int)sizeofArray(gNodeTypes); ++i)
			{
				if (ImGui::MenuItem(gNodeTypes[i].name))
				{
					_graph.AddNode(ImGui::GetIO().MousePos - graphOffset, gNodeTypes[i]);
				}
			}
	
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();
	}

	// Scrolling
	if (ImGui::IsWindowHovered() && 
		ImGui::IsAnyItemActive() == false && 
		ImGui::IsMouseDragging(2, 0.0f))
		graphOffset = graphOffset + ImGui::GetIO().MouseDelta;
		// graphOffset = graphOffset + ImGui::GetMouseDragDelta(2);

	ImGui::PopItemWidth();
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
	ImGui::EndGroup();

	ImGui::End();
}

void Application::Show() {
	this->ShowGraphEditor();
}