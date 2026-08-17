#pragma once

#include "imtk/color.hpp"

namespace imtk::overlays
{
	extern void quad_error(ImVec2 rect_start, ImVec2 rect_end, ImU32 color = col::error);
	extern void quad_warning(ImVec2 rect_start, ImVec2 rect_end, ImU32 color = col::warning, int bar_count = 5, float max_border_width = 10.f);
}
