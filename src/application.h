#ifndef NODE_PROGRAM_H
#define NODE_PROGRAM_H

#include <vector>
#include <stdint.h>

#include <imgui.h>
#include "imgui_dock.h"

class Application {
private:
	bool _isGraphEditorOpen;
	void ShowGraphEditor();
public:
	Application();
	void Init();
	void Show();
};

#endif // NODE_PROGRAM_H