#pragma once

#include <imgui.h>

namespace imtk
{
	class outline
	{
		ImVec2 _start_pos;

	public:
		outline();

		struct config
		{
			float rounding = 0.f;
			float border = 1.f;
			ImDrawFlags flags = 0;
		};

		void draw(ImU32 color, config cfg = {}) const;
	};
}
