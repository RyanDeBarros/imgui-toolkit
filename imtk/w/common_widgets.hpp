#pragma once

#include "imtk/w/widget.hpp"
#include "imtk/w/simple_widget.hpp"

#include "imtk/label_registry.hpp"

#include <imgui.h>

#include <functional>
#include <string>
#include <string_view>

namespace imtk::w
{
	struct generic_widget : public widget
	{
		std::function<item_result()> draw_fn;

		generic_widget(std::function<item_result()> draw_fn) : draw_fn(std::move(draw_fn)) {}

	protected:
		item_result draw_impl() override;
	};

	struct subsequent : public widget
	{
		std::unique_ptr<widget> item;

		subsequent(std::unique_ptr<widget>&& item) : item(std::move(item)) {}

	protected:
		item_result draw_impl();
	};

	struct optional_widget : public widget
	{
		simple_widget<bool> enable;
		std::unique_ptr<widget> item;

		optional_widget(std::unique_ptr<widget>&& item, bool& enable, simple_widget<bool>::config_impl config = {}) : enable(enable, std::move(config)), item(std::move(item)) {}

	protected:
		item_result draw_impl() override;
	};

	struct combo_widget : public widget
	{
		int& index;
		imtk::label_span_registry::handle names;

		struct config_impl
		{
			std::string label;
		} config;

		combo_widget(int& index, imtk::label_span_registry::handle names, config_impl config = {}) : index(index), names(names), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};
}
