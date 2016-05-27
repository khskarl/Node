#include "application.h"
#include "graph_drawing_utils.h"


const float NODE_SLOT_RADIUS = 6.0f;
const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);
const float SLOTS_SPACING = 4.0f;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint32_t s_id = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void SetupSlots(std::vector<Connection*>& connections, SlotDesc* connectionDescs)
{
	for (int i = 0; i < MAX_CONNECTION_COUNT; ++i)
	{
		const SlotDesc& desc = connectionDescs[i];

		if (!desc.name)
			break;

		Connection* con = new Connection;
		con->desc = desc;

		connections.push_back(con);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Node* CreateNodeFromType(ImVec2 pos, NodeType* nodeType)
{
	Node* node = new Node;
	node->id = s_id++;
	node->name = nodeType->name;

	ImVec2 titleSize = ImGui::CalcTextSize(node->name);

	titleSize.y *= 2.5f;

	SetupSlots(node->inputConnections, nodeType->inputConnections);
	SetupSlots(node->outputConnections, nodeType->outputConnections);

	// Calculate the size needed for the whole box

	ImVec2 inputTextSize(0.0f, 0.0f);
	ImVec2 outputText(0.0f, 0.0f);

	for (Connection* c : node->inputConnections)
	{
		ImVec2 textSize = ImGui::CalcTextSize(c->desc.name);
		inputTextSize.x = std::max<float>(textSize.x, inputTextSize.x);

		c->pos = ImVec2(0.0f, titleSize.y + inputTextSize.y + textSize.y / 2.0f);

		inputTextSize.y += textSize.y;
		inputTextSize.y += SLOTS_SPACING; // space between text entries
	}

	// inputTextSize.x += 40.0f;
	inputTextSize.x = 100.0f;
	inputTextSize.y = 100.0f;

	float xStart = inputTextSize.x;

	// Calculate for the outputs

	for (Connection* c : node->outputConnections)
	{
		ImVec2 textSize = ImGui::CalcTextSize(c->desc.name);
		inputTextSize.x = std::max<float>(xStart + textSize.x, inputTextSize.x);
	}

	node->pos = pos;
	node->size.x = inputTextSize.x;	
	node->size.y = inputTextSize.y + titleSize.y;

	inputTextSize.y = 0.0f;

	// set the positions for the output nodes when we know where the place them

	for (Connection* c : node->outputConnections)
	{		
		ImVec2 textSize = ImGui::CalcTextSize(c->desc.name);
		c->pos = ImVec2(node->size.x, titleSize.y + inputTextSize.y + textSize.y / 2.0f);

		inputTextSize.y += textSize.y;
		inputTextSize.y += SLOTS_SPACING; // space between text entries
	}

	// calculate the size of the node depending on number of connections

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
	Connection* con;
};

static DragNode s_dragNode;
static DragState s_dragState = DragState::Default;

static std::vector<Node*> s_nodes;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool IsSlotHovered(Connection* c, ImVec2 offset)
{
	ImVec2 mousePos = ImGui::GetIO().MousePos;
	ImVec2 conPos = offset + c->pos;

	float xd = mousePos.x - conPos.x;
	float yd = mousePos.y - conPos.y;

	return ((xd * xd) + (yd *yd)) < (NODE_SLOT_RADIUS * NODE_SLOT_RADIUS); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Connection* getHoverCon(ImVec2 offset, ImVec2* pos)
{
	for (Node* node : s_nodes)
	{
		ImVec2 nodePos = node->pos + offset;

		for (Connection* con : node->inputConnections)
		{
			if (IsSlotHovered(con, nodePos))
			{
				*pos = nodePos + con->pos;
				return con;
			}
		}

		for (Connection* con : node->outputConnections)
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
			Connection* con = getHoverCon(offset, &pos);

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
			Connection* con = getHoverCon(offset, &pos);

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
				Connection* con = getHoverCon(offset, &pos);

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

Node* findNodeByCon(Connection* findCon)
{
	for (Node* node : s_nodes)
	{
		for (Connection* con : node->inputConnections)
		{
			if (con == findCon)
				return node;
		}

		for (Connection* con : node->outputConnections)
		{
			if (con == findCon)
				return node;
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawConnections(ImDrawList* drawList, ImVec2 offset)
{
	for (Node* node : s_nodes)
	{
		for (Connection* con : node->inputConnections)
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
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor(40, 40, 40, 255));
	ImGui::BeginChild("scrolling_region", ImVec2(0,0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
	ImGui::PushItemWidth(120.0f);


	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->ChannelsSplit(2);

	for (Node* node : s_nodes)
		DrawNode(draw_list, scrolling, node, node_selected);

	updateDraging(scrolling);
	DrawConnections(draw_list, scrolling);

	draw_list->ChannelsMerge();

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