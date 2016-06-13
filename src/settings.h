#ifndef	SETTINGS_H
#define SETTINGS_H

#include <imgui.h>

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x+rhs.x, lhs.y+rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x-rhs.x, lhs.y-rhs.y); }


#define MAX_INPUT_COUNT 4

#define sizeofArray(t) (sizeof(t) / sizeof(t[0]))


namespace Settings {

const float SlotRadius = 6.0f;

const float SlotsSpacing = 4.0f;

const float NodeBorderSize = 4.0f;

const ImVec2 NodeWindowPadding(8.0f, 8.0f);
const ImVec2 NodeSize(120, 120);

const ImColor NodeColor(150, 150, 150);
const ImColor NodeBgColor(60, 60, 60);
const ImColor NodeBgHighlightColor(70, 70, 70);
const ImColor NodeBorderColor(100,100,100);
const ImColor NodeBorderHighlightColor(125,130,125);

const ImColor SlotColor(150, 150, 150);

const ImColor LinkColor(220, 220, 220);

const ImColor BackgroundColor(20, 20, 20, 255);

}

#endif // SETTINGS_H