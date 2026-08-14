#pragma once

#include "imtk/item_state.hpp"

#include <vector>
#include <string>

#include <imgui.h>

namespace imtk::controls
{
	extern void vertical_separator();
	extern item_result combo(const char* label, int& current_item, const std::vector<std::string>& items);
	extern item_result input_text(const char* label, std::string& string, size_t max_size = 256, ImGuiInputTextFlags flags = 0,
		ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
}
