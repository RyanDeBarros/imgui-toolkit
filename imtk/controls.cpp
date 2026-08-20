#include "controls.hpp"

#include "imtk/popup.hpp"

#include "imtk/prop/clipboard.hpp"
#include "imtk/prop/common_views.hpp"
#include "imtk/prop/property_grid.hpp"

#include <imgui_internal.h>

namespace imtk::controls
{
	void vertical_separator()
	{
		ImGui::SameLine();
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();
	}

	item_result combo(const char* label, int& current_item, const std::vector<std::string>& items)
	{
		float width = 0.0f;
		for (int i = 0; i < items.size(); i++)
			width = std::max(width, ImGui::CalcTextSize(items[i].c_str()).x);
		width += 2 * ImGui::GetFrameHeight(); // roughly covers dropdown arrow + padding
		ImGui::SetNextItemWidth(width);

		auto result = item_result::query(ImGui::Combo(label, &current_item, [](void* data, int idx) -> const char* {
			const std::vector<std::string>& items = *static_cast<const std::vector<std::string>*>(data);
			if (idx < 0 || idx >= items.size())
				return nullptr;
			else
				return items[idx].c_str();
			}, const_cast<std::vector<std::string>*>(&items), static_cast<int>(items.size())));

		result.modified |= prop::grid::check_property(std::make_unique<prop::dynamic_combo_view>(current_item, items));

		return result;
	}

	item_result input_text(const char* label, std::string& string, size_t max_size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
	{
		std::string buf;
		buf.resize(max_size);
		string.copy(buf.data(), buf.size());
		auto result = item_result::query(ImGui::InputText(label, buf.data(), buf.size(), flags, callback, user_data));
		if (result)
		{
			string = std::move(buf);
			size_t n = string.find('\0');
			if (n != std::string::npos)
				string.resize(n);
		}

		auto main_prop = std::make_unique<prop::simple_view<std::string>>(string);
		if (flags & ImGuiInputTextFlags_ReadOnly)
			result.modified |= prop::grid::check_property(std::make_unique<prop::readonly_view>(std::move(main_prop)));
		else
			result.modified |= prop::grid::check_property(std::move(main_prop));

		if (string.size() > max_size)
		{
			string.resize(max_size);
			result.modified = true;
		}

		return result;
	}

	item_result readonly_text(const char* label, std::string_view string, ImGuiInputTextFlags flags)
	{
		auto result = item_result::query(ImGui::InputText(label, const_cast<char*>(string.data()), string.size() + 1, flags | ImGuiInputTextFlags_ReadOnly));
		result.modified |= prop::grid::check_property(std::make_unique<prop::readonly_text_view>(std::string(string)));
		return result;
	}

	item_result float_popout(const char* label, float& value, float min, float max, const char* format, ImGuiSliderFlags slider_flags, float step, float step_fast)
	{
		auto property = std::make_unique<prop::simple_view<float>>(value);

		auto result = item_result::query(ImGui::SliderFloat(label, &value, min, max, format, slider_flags));
		result.modified |= prop::clipboard::context_menu(*property);

		popup popup("Edit value##" + std::string(label));

		if (result.state.right_clicked())
			popup.open();

		if (auto d = popup.draw())
		{
			result |= item_result::query(ImGui::InputFloat(label, &value, step, step_fast, format));
			result.modified |= prop::clipboard::context_menu(*property);
			value = std::max(value, min);
			value = std::min(value, max);
		}

		prop::grid::add_property(std::move(property));
		return result;
	}

	item_result int_popout(const char* label, int& value, int min, int max, const char* format, ImGuiSliderFlags slider_flags, int step, int step_fast)
	{
		auto property = std::make_unique<prop::simple_view<int>>(value);

		auto result = item_result::query(ImGui::SliderInt(label, &value, min, max, format, slider_flags));
		result.modified |= prop::clipboard::context_menu(*property);

		popup popup("Edit value##" + std::string(label));

		if (result.state.right_clicked())
			popup.open();

		if (auto d = popup.draw())
		{
			result |= item_result::query(ImGui::InputInt(label, &value, step, step_fast));
			result.modified |= prop::clipboard::context_menu(*property);
			value = std::max(value, min);
			value = std::min(value, max);
		}

		prop::grid::add_property(std::move(property));
		return result;
	}
}
