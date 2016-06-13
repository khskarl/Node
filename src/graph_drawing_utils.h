#ifndef GRAPH_DRAWING_UTILS_H
#define GRAPH_DRAWING_UTILS_H

#include <imgui.h>
#include "graph.h"


void SetDrawingOffset(ImVec2 offset);
void DrawHermite(ImDrawList* drawList, ImVec2 p1, ImVec2 p2);
void DrawLinks(ImDrawList* drawList, Graph& graph);
void DrawNode(ImDrawList* drawList, Node* node, int& node_selected);
bool IsSlotHovered(Slot* slot);
#endif