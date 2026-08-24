#include "widget_row.hpp"

#include "imtk/simple_scopes.hpp"
#include "imtk/controls.hpp"

namespace imtk::w
{
	static item_result draw_row(const auto& subwidgets, bool separators)
	{
		item_result result;

		style_var cell_padding(ImGuiStyleVar_CellPadding, ImVec2(ImGui::GetStyle().CellPadding.x, 0.f));
		ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_PreciseWidths;

		if (separators)
			flags |= ImGuiTableFlags_BordersInnerV;

		if (auto _ = table("##InlineWidget", subwidgets.size(), flags,
			ImVec2(-FLT_MIN, ImGui::GetFrameHeight())))
		{
			ImGui::TableNextRow();

			for (size_t i = 0; i < subwidgets.size(); ++i)
			{
				ImGui::TableNextColumn();
				if (auto _ = item_width_scope(expand_item_width{}))
					result |= subwidgets[i]->draw();
			}
		}

		return result;
	}

	item_result bound_widget_row::draw_impl()
	{
		return draw_row(subwidgets, config.separators);
	}

	item_result widget_row::draw_impl()
	{
		return draw_row(subwidgets, config.separators);
	}
}
