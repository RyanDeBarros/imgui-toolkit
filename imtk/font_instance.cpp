#include "font_instance.hpp"

namespace imtk
{
	font_instance::font_instance(const char* filename, float size_pixels)
	{
		_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename, size_pixels);
	}

	font_instance::font_instance(font_instance&& o) noexcept
		: _font(o._font)
	{
		o._font = nullptr;
	}
	
	font_instance::~font_instance()
	{
		reset();
	}

	font_instance& font_instance::operator=(font_instance&& o) noexcept
	{
		if (this != &o)
		{
			reset();
			_font = o._font;
			o._font = nullptr;
		}

		return *this;
	}

	void font_instance::reset()
	{
		if (_font)
		{
			ImGui::GetIO().Fonts->RemoveFont(_font);
			_font = nullptr;
		}
	}

	font_instance::operator const ImFont*() const
	{
		return _font;
	}

	font_instance::operator ImFont*()
	{
		return _font;
	}

	font_instance::operator bool() const
	{
		return _font != nullptr;
	}
}
