#ifndef NODE_PROGRAM_H
#define NODE_PROGRAM_H

#include <vector>
#include <stdint.h>

#include <imgui.h>
#include "imgui_dock.h"
#include "graph.h"

class Application {
private:
	bool _isGraphEditorOpen;
	void ShowGraphEditor();
	ImVec2 graphOffset;
	Graph _graph;
public:
	Application();
	void Init();
	void Show();
};

#endif // NODE_PROGRAM_H