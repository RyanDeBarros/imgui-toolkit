#pragma once

#include <vector>
#include <string>

#include <imgui.h>

namespace imtk::controls
{
	extern void vertical_separator();
	extern bool combo(const char* label, int& current_item, const std::vector<std::string>& items);
	extern bool input_text(const char* label, std::string& string, size_t max_size = 256, ImGuiInputTextFlags flags = 0,
		ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
}
