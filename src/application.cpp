#include "application.h"

#include <iostream>

#include "settings.h"
#include "graph_drawing_utils.h"

#include "graph.h"

using std::cout;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum DragState 
{
	Default,
	Hover,
	BeginDrag,
	Draging,
	Connect,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct DragNode
{
	ImVec2 pos;
	Link* con;
};

static DragNode s_dragNode;
static DragState s_dragState = DragState::Default;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Link* getHoverCon(ImVec2 offset, ImVec2* pos)
{
	// for (Node* node : s_nodes)
	// {
	// 	ImVec2 nodePos = node->pos + offset;

	// 	for (Slot* con : node->input)
	// 	{
	// 		if (IsSlotHovered(con, nodePos))
	// 		{
	// 			*pos = nodePos + con->pos;
	// 			return con;
	// 		}
	// 	}

	// 	for (Link* con : node->outputLinks)
	// 	{
	// 		if (IsSlotHovered(con, nodePos))
	// 		{
	// 			*pos = nodePos + con->pos;
	// 			return con;
	// 		}
	// 	}
	// }
	
	// s_dragNode.con = 0;
	return 0; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void updateDraging(ImVec2 offset)
{
	// switch (s_dragState)
	// {
	// 	case Default:
	// 	{
	// 		ImVec2 pos;
	// 		Link* con = getHoverCon(offset, &pos);

	// 		if (con)
	// 		{
	// 			s_dragNode.con = con;
	// 			s_dragNode.pos = pos; 
	// 			s_dragState = Hover;
	// 		}
				
	// 		break;
	// 	}

	// 	case Hover:
	// 	{
	// 		ImVec2 pos;
	// 		Link* con = getHoverCon(offset, &pos);

	// 		// Make sure we are still hovering the same node

	// 		if (con != s_dragNode.con)
	// 		{
	// 			s_dragNode.con = 0;
	// 			s_dragState = Default;
	// 			return;
	// 		}

	// 		if (ImGui::IsMouseClicked(0) && s_dragNode.con) 
	// 			s_dragState = Draging;

	// 		break;
	// 	}

	// 	case BeginDrag:
	// 	{
	// 		break;
	// 	}

	// 	case Draging:
	// 	{
	// 		ImDrawList* drawList = ImGui::GetWindowDrawList();

	// 		drawList->ChannelsSetCurrent(0); // Background

	// 		// Holding connection
	// 		DrawHermite(drawList, s_dragNode.pos, ImGui::GetIO().MousePos);

	// 		if (!ImGui::IsMouseDown(0))
	// 		{
	// 			ImVec2 pos;
	// 			Link* con = getHoverCon(offset, &pos);

	// 			// Make sure we are still hovering the same node

	// 			if (con == s_dragNode.con)
	// 			{
	// 				s_dragNode.con = 0;
	// 				s_dragState = Default;
	// 				return;
	// 			}

	// 			// Lets connect the nodes.
	// 			// TODO: Make sure we connect stuff in the correct way!

	// 			scon->input = s_dragNode.con;
	// 			s_dragNode.con = 0;
	// 			s_dragState = Default;
	// 		}

	// 		break;
	// 	}

	// 	case Connect:
	// 	{
	// 		break;
	// 	}
	// }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Node* findNodeByCon(Link* findCon)
{
	// for (Node* node : s_nodes)
	// {
	// 	for (Link* con : node->inputLinks)
	// 	{
	// 		if (con == findCon)
	// 			return node;
	// 	}

	// 	for (Link* con : node->outputLinks)
	// 	{
	// 		if (con == findCon)
	// 			return node;
	// 	}
	// }

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////

Application::Application() {

}

void Application::Init() {
	_graph.AddNode(ImVec2(100, 200), gNodeTypes[0]);
	_graph.AddNode(ImVec2(600, 200), gNodeTypes[1]);
	_graph.AddNode(ImVec2(300, 300), gNodeTypes[2]);
	Slot* from = _graph.GetNodeData()[0]->output;
	Slot* to = _graph.GetNodeData()[1]->	inputs[0];

	_graph.AddLink(from, to);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Application::ShowGraphEditor()
{
	ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Graph Editor", &_isGraphEditorOpen);

	// TODO: Remove/refactor those
	int node_hovered_in_list = -1;
	int node_hovered_in_scene = -1;

	static int node_selected = -1;
	static ImVec2 scrollOffset = ImVec2(0.0f, 0.0f);
	// End TODO

	ImGui::SameLine();
	ImGui::BeginGroup();

	// Create our child canvas
	// ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", scrollOffset.x, scrollOffset.y);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, Settings::BackgroundColor);
	ImGui::BeginChild("scroll_region", 
					  ImVec2(0,0), 
					  true, 
					  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
	ImGui::PushItemWidth(120.0f);


	ImDrawList* drawList = ImGui::GetWindowDrawList();

	std::vector<Node*> nodes = _graph.GetNodeData();
	for (Node * node : nodes)
		DrawNode(drawList, scrollOffset, node, node_selected);

	// updateDraging(scrollOffset);
	DrawLinks(drawList, _graph, scrollOffset);

	// Open context menu
	bool open_context_menu = false;
	if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(1))
	{
		node_selected = node_hovered_in_list = node_hovered_in_scene = -1;
		open_context_menu = true;
	}
	if (open_context_menu)
	{
		ImGui::OpenPopup("context_menu");
		if (node_hovered_in_list != -1)
			node_selected = node_hovered_in_list;
		if (node_hovered_in_scene != -1)
			node_selected = node_hovered_in_scene;
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
					_graph.AddNode(ImGui::GetIO().MousePos, gNodeTypes[i]);
				}
			}
	
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();
	}

	// Scrolling
	if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
		scrollOffset = scrollOffset + ImGui::GetIO().MouseDelta;

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