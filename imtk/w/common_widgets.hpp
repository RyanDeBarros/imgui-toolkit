#pragma once

#include "imtk/w/widget.hpp"

#include <functional>
#include <string>

namespace imtk::w
{
	struct generic_widget : public widget
	{
		std::function<item_result()> draw_fn;

		generic_widget(std::function<item_result()> draw_fn);

	protected:
		item_result draw_impl() override;
	};

	struct text : public widget
	{
		std::string text;

	protected:
		item_result draw_impl() override;
	};

	struct text_view : public widget
	{
		std::string_view text;

	protected:
		item_result draw_impl() override;
	};
}
