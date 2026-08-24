#pragma once

#include "imtk/w/widget.hpp"

#include <memory>
#include <span>
#include <vector>

namespace imtk::w
{
	struct bound_widget_row : public widget
	{
		std::span<widget*> subwidgets;

		struct config_impl
		{
			bool separators = false;
		} config;

		bound_widget_row(std::span<widget*> subwidgets, config_impl config = {}) : subwidgets(std::move(subwidgets)), config(std::move(config)) {}

	protected:
		item_result draw_impl();
	};

	struct widget_row : public widget
	{
		std::vector<std::unique_ptr<widget>> subwidgets;

		struct config_impl
		{
			bool separators = false;
		} config;

		widget_row(config_impl config = {}) : config(std::move(config)) {}

	protected:
		item_result draw_impl();
	};
}
