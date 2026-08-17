#pragma once

#include <ostream>

#include <imgui.h>

namespace imtk
{
	struct color4
	{
		union
		{
			struct { float r, g, b, a; };
			float v[4];
		};

		color4();
		color4(float r, float g, float b, float a);

		bool operator==(const color4&) const;
		bool operator!=(const color4&) const;

		float* ptr();
		const float* ptr() const;
		float& operator[](size_t i);
		float operator[](size_t i) const;

		friend std::ostream& operator<<(std::ostream& os, color4 color);
	};

	namespace col
	{
		enum : ImU32
		{
			azure = IM_COL32(0, 127, 255, 255),
			black = IM_COL32(0, 0, 0, 255),
			blue = IM_COL32(0, 0, 255, 255),
			green = IM_COL32(0, 255, 0, 255),
			magenta = IM_COL32(255, 0, 255, 255),
			red = IM_COL32(255, 0, 0, 255),
			white = IM_COL32(255, 255, 255, 255),
			yellow = IM_COL32(255, 255, 0, 255),

			error = red,
			success = green,
			warning = yellow,
		};

		constexpr static ImU32 grey(unsigned char level)
		{
			return IM_COL32(level, level, level, 255);
		}
	}
}
