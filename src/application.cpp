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

	// Slot* from = _graph.GetNodeData()[0]->output;
	// Slot* to   = _graph.GetNodeData()[1]->inputs[0];
	// _graph.AddLink(from, to);

}

void Application::UpdateGraphInteraction(ImDrawList* drawList) {

	Slot* hoveredSlot = GetHoveredSlot(_graph);
	SetHoveredSlot(hoveredSlot);
	Node* hoveredNode = GetHoveredNode(_graph); 
	SetHoveredNode(hoveredNode);

	ImGui::SetCursorScreenPos(ImGui::GetWindowPos());

	static Slot* otherSlot = nullptr;

	switch(_linkDragState) {
	case LinkDragState::Idle:
		if (hoveredNode != nullptr)
			_linkDragState = LinkDragState::HoveringNode;

		if (hoveredSlot != nullptr) 
			_linkDragState = LinkDragState::HoveringSlot;

		ImGui::Text("Idle");
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
	
		ImGui::Text("Hovering Slot");
		break;

	case LinkDragState::HoveringNode:
		if (hoveredNode == nullptr) {
			_linkDragState = LinkDragState::Idle;
			break;
		}

		if (ImGui::IsMouseClicked(0) == true) 
		{
			_linkDragState = LinkDragState::DragingNode;
		}
	
		ImGui::Text("Hovering Node");
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
					otherSlot->GetWorldPos() + ImGui::GetWindowPos(), 
					ImGui::GetIO().MousePos);
		
		ImGui::Text("Draging Input Link");
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
					otherSlot->GetWorldPos() + ImGui::GetWindowPos());
		
		ImGui::Text("Draging Output Link");
		break;

	case LinkDragState::DragingNode:
		if (ImGui::IsMouseReleased(0) || hoveredNode == nullptr) { 			
			_linkDragState = LinkDragState::Idle;
			break;
		}
		else if (ImGui::IsMouseDown(0)) {
			DragNode(hoveredNode);
		}
		
		ImGui::Text("Draging Node");
		break;

	default:
		_linkDragState = LinkDragState::Idle;
		break;
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Application::ShowGraphEditor()
{
	ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Graph Editor", &_isGraphEditorOpen);

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
	SetDrawingOffset(graphOffset + ImGui::GetWindowPos()); 

	ImDrawList* drawList = ImGui::GetWindowDrawList();

	UpdateGraphInteraction(drawList);
	
	DrawLinks(drawList, _graph);
	DrawNodes(drawList, _graph);

	////////////////////

	// Open context menu
	bool openContextMenu = false;
	if (ImGui::IsAnyItemHovered() == false && ImGui::IsMouseHoveringWindow() && 
		ImGui::IsMouseClicked(1))
	{
		openContextMenu = true;
	}

	if (openContextMenu)
	{
		ImGui::OpenPopup("context_menu");
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
					ImVec2 nodePos = ImGui::GetIO().MousePos - graphOffset;
					_graph.AddNode(nodePos, gNodeTypes[i]);
				}
			}
	
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();
	}

	// Scrolling
	if (ImGui::IsWindowHovered() && ImGui::IsAnyItemActive() == false && 
		ImGui::IsMouseDragging(2, 0.0f))
		graphOffset = graphOffset + ImGui::GetIO().MouseDelta;
	
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