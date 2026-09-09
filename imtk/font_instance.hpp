#pragma once

#include <imgui.h>

namespace imtk
{
	class font_instance
	{
		ImFont* _font = nullptr;

	public:
		font_instance() = default;
		font_instance(const char* filename, float size_pixels = 0.f);
		font_instance(const font_instance&) = delete;
		font_instance(font_instance&&) noexcept;
		~font_instance();
		font_instance& operator=(const font_instance&) = delete;
		font_instance& operator=(font_instance&&) noexcept;

		void reset();

		operator const ImFont*() const;
		operator ImFont*();
		operator bool() const;
	};
}
