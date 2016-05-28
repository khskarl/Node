#ifndef	SETTINGS_H
#define SETTINGS_H

#include <imgui.h>

namespace Settings {

const float SlotRadius = 6.0f;
const float SlotsSpacing = 4.0f;

const float NodeBorderSize = 4.0f;

const ImVec2 NodeWindowPadding(8.0f, 8.0f);
const ImVec2 NodeSize(120, 120);

const ImColor NodeColor(150, 150, 150);
const ImColor NodeBgColor(60, 60, 60);
const ImColor NodeBgHighlightColor(60, 60, 60);
const ImColor NodeBorderColor(100,100,100);
const ImColor NodeBorderHighlightColor(125,125,125);

const ImColor SlotColor(150, 150, 150);

const ImColor LinkColor(220, 220, 220);

}

#endif // SETTINGS_H