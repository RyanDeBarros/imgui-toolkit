#include "overlays.hpp"

namespace imtk::overlays
{
	void quad_error(ImVec2 rect_start, ImVec2 rect_end, ImU32 color)
	{
		ImGui::GetWindowDrawList()->AddRectFilled(rect_start, rect_end, color);
	}

	void quad_warning(ImVec2 rect_start, ImVec2 rect_end, ImU32 color, int bar_count, float max_border_width)
	{
		const int bar_multiplier = 2 * bar_count - 1;
		const float threshold_width = max_border_width * bar_multiplier;

		const ImVec2 size = rect_end - rect_start;
		const float min_dimension = std::min(size.x, size.y);

		float border_width = max_border_width;
		if (min_dimension < threshold_width)
			border_width = min_dimension / bar_multiplier;

		float x_gap_width = max_border_width;
		if (size.x > threshold_width)
			x_gap_width = (size.x - (bar_count * border_width)) / (bar_count - 1);

		float y_gap_width = max_border_width;
		if (size.y > threshold_width)
			y_gap_width = (size.y - (bar_count * border_width)) / (bar_count - 1);

		// Vertical bars
		ImGui::GetWindowDrawList()->AddRectFilled(rect_start, rect_start + ImVec2(border_width, size.y), color);
		ImGui::GetWindowDrawList()->AddRectFilled(rect_end - ImVec2(border_width, size.y), rect_end, color);
		for (int i = 1; i < bar_count - 1; ++i)
		{
			ImVec2 bar_start = rect_start + ImVec2(i * (border_width + x_gap_width), 0);
			const ImVec2 p1 = bar_start + ImVec2(0.4f * border_width, size.y);
			const ImVec2 p2 = bar_start + ImVec2(0.f, 0.f);
			const ImVec2 p3 = bar_start + ImVec2(border_width, 0.f);
			const ImVec2 p4 = bar_start + ImVec2(0.6f * border_width, size.y);

			ImGui::GetWindowDrawList()->AddTriangleFilled(p1, p2, p3, color);
			ImGui::GetWindowDrawList()->AddTriangleFilled(p1, p3, p4, color);
		}

		// Horizontal bars
		ImGui::GetWindowDrawList()->AddRectFilled(rect_start, rect_start + ImVec2(size.x, border_width), color);
		ImGui::GetWindowDrawList()->AddRectFilled(rect_end - ImVec2(size.x, border_width), rect_end, color);
		for (int i = 1; i < bar_count - 1; ++i)
		{
			ImVec2 bar_start = rect_start + ImVec2(0, i * (border_width + y_gap_width));
			const ImVec2 p1 = bar_start + ImVec2(0.f, 0.4f * border_width);
			const ImVec2 p2 = bar_start + ImVec2(size.x, 0.f);
			const ImVec2 p3 = bar_start + ImVec2(size.x, border_width);
			const ImVec2 p4 = bar_start + ImVec2(0.f, 0.6f * border_width);

			ImGui::GetWindowDrawList()->AddTriangleFilled(p1, p2, p3, color);
			ImGui::GetWindowDrawList()->AddTriangleFilled(p1, p3, p4, color);
		}
	}
}
