#include "widget_row.hpp"

#include "imtk/simple_scopes.hpp"

namespace imtk::w
{
	item_result widget_row::draw_impl()
	{
		item_result result;

		style_var cell_padding(ImGuiStyleVar_CellPadding, ImVec2(ImGui::GetStyle().CellPadding.x, 0.f));

		if (auto _ = table("##InlineWidget", subwidgets.size(), ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_PreciseWidths,
			ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFrameHeight())))
		{
			ImGui::TableNextRow();

			for (auto& subwidget : subwidgets)
			{
				ImGui::TableNextColumn();
				if (auto _ = item_width_scope(expand_item_width{}))
					result |= subwidget->draw();
			}
		}

		return result;
	}
}
