#ifndef NODE_PROGRAM_H
#define NODE_PROGRAM_H

#include <vector>
#include <stdint.h>

#include <imgui.h>
#include "imgui_dock.h"
#include "graph.h"


enum LinkDragState 
{
	Idle,
	HoveringSlot,
	HoveringNode,
	DragingInputLink,
	DragingOutputLink,
	DragingNode,
};

class Application {
private:
	ImVec2 graphOffset;
	Graph _graph;
	Node* hoveredNode = nullptr;
	Node* selectedNode = nullptr;
	Node* outputNode = nullptr;
	Slot* hoveredSlot;

	LinkDragState _linkDragState = LinkDragState::Idle;

	bool _isGraphEditorOpen;
	
	void UpdateGraphInteraction(ImDrawList* drawList);
	void ShowGraphEditor();
	
public:
	Application();
	void Init();
	void Show();
};

#endif // NODE_PROGRAM_H