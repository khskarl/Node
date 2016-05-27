#include "graph_drawing_utils.h"

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

