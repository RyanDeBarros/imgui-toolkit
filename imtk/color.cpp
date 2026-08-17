#include "color.hpp"

#include <string>

namespace imtk
{
	color4::color4()
		: r(0.f), g(0.f), b(0.f), a(1.f)
	{
	}

	color4::color4(float r, float g, float b, float a)
		: r(r), g(g), b(b), a(a)
	{
	}

	bool color4::operator==(const color4& o) const
	{
		return r == o.r && g == o.g && b == o.b && a == o.a;
	}

	bool color4::operator!=(const color4& o) const
	{
		return r != o.r || g != o.g || b != o.b || a != o.a;
	}

	float* color4::ptr()
	{
		return v;
	}

	const float* color4::ptr() const
	{
		return v;
	}

	float& color4::operator[](size_t i)
	{
		if (i < 4)
			return v[i];
		else
			throw std::out_of_range(std::to_string(i) + " is invalid index for color4");
	}

	float color4::operator[](size_t i) const
	{
		if (i < 4)
			return v[i];
		else
			throw std::out_of_range(std::to_string(i) + " is invalid index for color4");
	}

	std::ostream& operator<<(std::ostream& os, color4 color)
	{
		return os << "imtk::color4(" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")";
	}
}
