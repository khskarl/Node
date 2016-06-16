#ifndef GRAPH_DRAWING_UTILS_H
#define GRAPH_DRAWING_UTILS_H

#include <imgui.h>
#include "graph.h"


void DragNode(Node* node);
void SetDrawingOffset(ImVec2 offset);
void SetHoveredNode(Node* pHoveredNode);
void SetHoveredSlot(Slot* pHoveredSlot);
void DrawHermite(ImDrawList* drawList, ImVec2 p1, ImVec2 p2);
void DrawLinks(ImDrawList* drawList, Graph& graph);
void DrawNode(ImDrawList* drawList, Node* node);
void DrawNodes(ImDrawList* drawList, Graph& graph);
bool IsSlotHovered(Slot* slot);

Node* GetHoveredNode(Graph & graph);
Slot* GetHoveredSlot(Graph & graph);
#endif