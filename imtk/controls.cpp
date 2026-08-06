#include "controls.hpp"

#include "imtk/popup.hpp"

namespace imtk
{
	bool float_control(const char* label, float& value, float min, float max, const char* format, ImGuiSliderFlags slider_flags, float step, float step_fast)
	{
		bool changed = ImGui::SliderFloat(label, &value, min, max, format, slider_flags);

		popup popup("Edit value##" + std::string(label));

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			popup.open();

		if (auto d = popup.draw())
		{
			changed |= ImGui::InputFloat(label, &value, step, step_fast, format);
			value = std::max(value, min);
			value = std::min(value, max);
		}

		return changed;
	}

	bool int_control(const char* label, int& value, int min, int max, const char* format, ImGuiSliderFlags slider_flags, int step, int step_fast)
	{
		bool changed = ImGui::SliderInt(label, &value, min, max, format, slider_flags);

		popup popup("Edit value##" + std::string(label));

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			popup.open();

		if (auto d = popup.draw())
		{
			changed |= ImGui::InputInt(label, &value, step, step_fast);
			value = std::max(value, min);
			value = std::min(value, max);
		}

		return changed;
	}
}
