#pragma once

#include <ostream>

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
}
