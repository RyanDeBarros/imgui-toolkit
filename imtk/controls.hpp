#pragma once

#include <imgui.h>

namespace imtk
{
	extern bool float_control(const char* label, float& value, float min, float max, const char* format = "%.3f",
		ImGuiSliderFlags slider_flags = 0, float step = 0.f, float step_fast = 0.f);

	extern bool int_control(const char* label, int& value, int min, int max, const char* format = "%d",
		ImGuiSliderFlags slider_flags = 0, int step = 1, int step_fast = 100);
}
