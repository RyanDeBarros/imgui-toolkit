#pragma once

#include "imtk/item_result.hpp"

#include <vector>
#include <string>

#include <imgui.h>

namespace imtk::controls
{
	extern void vertical_separator();
	
	extern item_result combo(const char* label, int& current_item, const std::vector<std::string>& items);
	
	extern item_result input_text(const char* label, std::string& string, size_t max_size = 256, ImGuiInputTextFlags flags = 0,
		ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);

	extern item_result readonly_text(const char* label, std::string_view string, ImGuiInputTextFlags flags = 0);

	// TODO return item_result
	extern bool float_popout(const char* label, float& value, float min, float max, const char* format = "%.3f",
		ImGuiSliderFlags slider_flags = 0, float step = 0.f, float step_fast = 0.f);

	// TODO return item_result
	extern bool int_popout(const char* label, int& value, int min, int max, const char* format = "%d",
		ImGuiSliderFlags slider_flags = 0, int step = 1, int step_fast = 100);
}
