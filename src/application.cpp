#include "application.h"

#include <iostream>

#include "settings.h"
#include "graph_drawing_utils.h"
#include "node_types.h"
#include "graph.h"

using std::cout;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint32_t s_id = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void SetupSlots(std::vector<Link*>& connections, SlotDesc* connectionDescs)
{
	for (int i = 0; i < MAX_CONNECTION_COUNT; ++i)
	{
		const SlotDesc& desc = connectionDescs[i];

		if (!desc.name)
			break;

		Link* con = new Link;
		con->desc = desc;

		connections.push_back(con);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Node* CreateNodeFromType(ImVec2 pos, NodeType* nodeType)
{
	Node* node = new Node;
	node->id   = s_id++;
	node->name = nodeType->name;

	SetupSlots(node->inputLinks, nodeType->inputLinks);
	SetupSlots(node->outputLinks, nodeType->outputLinks);

	// Set slots positions
	{
		float slotHeight = 0;
		float dx = Settings::NodeSize.y / node->inputLinks.size();
		for (Link* link : node->inputLinks)
		{
			link->pos = ImVec2(0.0f, slotHeight + dx / 2.f);
			slotHeight += dx;
		}
	}

	node->pos = pos;
	node->size.x = Settings::NodeSize.x;	
	node->size.y = Settings::NodeSize.y;

	// set the positions for the output nodes
	for (Link* l : node->outputLinks)
	{		
		l->pos = ImVec2(node->size.x, node->size.y / 2.f);
	}

	return node;
}

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

static std::vector<Node*> s_nodes;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Link* getHoverCon(ImVec2 offset, ImVec2* pos)
{
	for (Node* node : s_nodes)
	{
		ImVec2 nodePos = node->pos + offset;

		for (Link* con : node->inputLinks)
		{
			if (IsSlotHovered(con, nodePos))
			{
				*pos = nodePos + con->pos;
				return con;
			}
		}

		for (Link* con : node->outputLinks)
		{
			if (IsSlotHovered(con, nodePos))
			{
				*pos = nodePos + con->pos;
				return con;
			}
		}
	}
	
	s_dragNode.con = 0;
	return 0; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void updateDraging(ImVec2 offset)
{
	switch (s_dragState)
	{
		case Default:
		{
			ImVec2 pos;
			Link* con = getHoverCon(offset, &pos);

			if (con)
			{
				s_dragNode.con = con;
				s_dragNode.pos = pos; 
				s_dragState = Hover;
			}
				
			break;
		}

		case Hover:
		{
			ImVec2 pos;
			Link* con = getHoverCon(offset, &pos);

			// Make sure we are still hovering the same node

			if (con != s_dragNode.con)
			{
				s_dragNode.con = 0;
				s_dragState = Default;
				return;
			}

			if (ImGui::IsMouseClicked(0) && s_dragNode.con) 
				s_dragState = Draging;

			break;
		}

		case BeginDrag:
		{
			break;
		}

		case Draging:
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			drawList->ChannelsSetCurrent(0); // Background

			// Holding connection
			DrawHermite(drawList, s_dragNode.pos, ImGui::GetIO().MousePos);

			if (!ImGui::IsMouseDown(0))
			{
				ImVec2 pos;
				Link* con = getHoverCon(offset, &pos);

				// Make sure we are still hovering the same node

				if (con == s_dragNode.con)
				{
					s_dragNode.con = 0;
					s_dragState = Default;
					return;
				}

				// Lets connect the nodes.
				// TODO: Make sure we connect stuff in the correct way!

				con->input = s_dragNode.con;
				s_dragNode.con = 0;
				s_dragState = Default;
			}

			break;
		}

		case Connect:
		{
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Node* findNodeByCon(Link* findCon)
{
	for (Node* node : s_nodes)
	{
		for (Link* con : node->inputLinks)
		{
			if (con == findCon)
				return node;
		}

		for (Link* con : node->outputLinks)
		{
			if (con == findCon)
				return node;
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawLinks(ImDrawList* drawList, ImVec2 offset)
{
	for (Node* node : s_nodes)
	{
		for (Link* con : node->inputLinks)
		{
			if (!con->input)
				continue;

			Node* targetNode = findNodeByCon(con->input);

			if (!targetNode)
				continue;

			DrawHermite(drawList, 
						offset + targetNode->pos + con->input->pos, 
						offset + node->pos + con->pos);
		}
	}
}
///////////////////

Application::Application() {

}

void Application::Init() {
	std::cout << "LADSDASDASDOL";
	Node* node = CreateNodeFromType(ImVec2(100, 200), &s_nodeTypes[0]);
	s_nodes.push_back(node);

	node = CreateNodeFromType(ImVec2(600, 200), &s_nodeTypes[1]);
	s_nodes.push_back(node);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Application::ShowGraphEditor()
{
	ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Graph Editor", &_isGraphEditorOpen);

	bool open_context_menu = false;
	int node_hovered_in_list = -1;
	int node_hovered_in_scene = -1;

	static int node_selected = -1;
	static ImVec2 scrolling = ImVec2(0.0f, 0.0f);

	ImGui::SameLine();
	ImGui::BeginGroup();

	// Create our child canvas
	//ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", scrolling.x, scrolling.y);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, Settings::BackgroundColor);
	ImGui::BeginChild("scroll_region", 
					  ImVec2(0,0), 
					  true, 
					  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
	ImGui::PushItemWidth(120.0f);


	ImDrawList* drawList = ImGui::GetWindowDrawList();

	for (Node* node : s_nodes)
		DrawNode(drawList, scrolling, node, node_selected);

	updateDraging(scrolling);
	DrawLinks(drawList, scrolling);

	// Open context menu
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
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8,8));
	if (ImGui::BeginPopup("context_menu"))
	{

		for (int i = 0; i < (int)sizeofArray(s_nodeTypes); ++i)
		{
			if (ImGui::MenuItem(s_nodeTypes[i].name))
			{
				Node* node = CreateNodeFromType(ImGui::GetIO().MousePos, &s_nodeTypes[i]);
				s_nodes.push_back(node);
			}
		}

		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();

	// Scrolling
	if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
		scrolling = scrolling + ImGui::GetIO().MouseDelta;

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