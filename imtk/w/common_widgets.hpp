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

	struct bound_optional : public widget
	{
		bound_widget<bool> enable;
		std::unique_ptr<widget> item;

		bound_optional(std::unique_ptr<widget>&& item, bool& enable, bound_widget<bool>::config_impl config = {}) : enable(enable, std::move(config)), item(std::move(item)) {}

	protected:
		item_result draw_impl() override;
	};

	struct simple_optional : public widget
	{
		simple_widget<bool> enable;
		std::unique_ptr<widget> item;

		simple_optional(std::unique_ptr<widget>&& item, bool& enable, simple_widget<bool>::config_impl config = {}) : enable(enable, std::move(config)), item(std::move(item)) {}

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

	struct readonly_text : public widget
	{
		std::string& text;

		struct config_impl
		{
			std::string label;

			ImGuiInputTextFlags flags = 0;
		} config;

		readonly_text(std::string& text, config_impl config = {}) : text(text), config(std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};

	struct readonly_text_owned : public widget
	{
		std::string text;
		readonly_text view;

		readonly_text_owned(std::string text, readonly_text::config_impl config = {}) : text(std::move(text)), view(this->text, std::move(config)) {}

	protected:
		item_result draw_impl() override;
	};
}
