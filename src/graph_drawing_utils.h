#ifndef GRAPH_DRAWING_UTILS_H
#define GRAPH_DRAWING_UTILS_H

#include <imgui.h>
#include "graph.h"

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x+rhs.x, lhs.y+rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x-rhs.x, lhs.y-rhs.y); }


void DrawHermite(ImDrawList* drawList, ImVec2 p1, ImVec2 p2);
void DrawLinks(ImDrawList* drawList, Graph& graph, ImVec2 offset);
void DrawNode(ImDrawList* drawList, ImVec2 offset, Node* node, int& node_selected);
bool IsSlotHovered(Link* c, ImVec2 parentPos);
#endif