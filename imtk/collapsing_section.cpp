#include "collapsing_section.hpp"

namespace imtk
{
	static int bg_toggle = 0;

	collapsing_section::collapsing_section(const char* label, bool start_open)
	{
		style_var sv(ImGuiStyleVar_ChildBorderSize, 2.f);

		ImVec4 border_col = ImGui::GetStyle().Colors[ImGuiCol_Border];
		const float w = border_col.w;
		border_col *= 1.f + 0.35f * bg_toggle++;
		border_col.w = w;
		style_color sc(ImGuiCol_Border, ImGui::GetColorU32(border_col));

		_child = std::make_unique<child>(label, ImVec2(0, 0), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Borders);
		if (*_child)
		{
			if (start_open)
				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
			_visible = ImGui::TreeNode(label);
		}
	}

	collapsing_section::collapsing_section(collapsing_section&& other) noexcept
		: _visible(other._visible)
	{
		other._visible = false;
		other._valid = false;
	}

	collapsing_section::~collapsing_section()
	{
		if (_valid)
		{
			if (_visible)
				ImGui::TreePop();

			_child.reset();
			--bg_toggle;
		}
	}

	collapsing_section::operator bool() const
	{
		return _valid && _visible;
	}
}
