#include "outline.hpp"

namespace imtk
{
	outline::outline()
		: _start_pos(ImGui::GetCursorScreenPos())
	{
	}

	void outline::draw(ImU32 color, config cfg) const
	{
		ImVec2 end_pos(ImGui::GetItemRectMax().x, ImGui::GetItemRectMin().y + ImGui::GetFrameHeight());
		ImGui::GetWindowDrawList()->AddRect(_start_pos, end_pos, color, cfg.rounding, cfg.flags, cfg.border);
	}
}
