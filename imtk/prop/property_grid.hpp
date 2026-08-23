#pragma once

#include "imtk/resource_loader.hpp"

#include "imtk/prop/view.hpp"
#include "imtk/w/widget.hpp"

#include <imgui.h>

#include <imp/instance_guard.hpp>
#include <imp/instance_stack.hpp>

namespace imtk::prop
{
	namespace key
	{
		extern void set_label(const std::string_view label);
	}

	namespace value
	{
		extern item_result get_draw_result();

		extern void add_component(std::unique_ptr<w::widget> component);
	}

	namespace reset
	{
		extern void button(size_t subrow = 0);
		extern bool activated(size_t subrow);
		extern bool any_activated();
	}

	namespace row
	{
		extern item_result get_draw_result();

		extern void submit();
		extern bool dirty();
	}

	struct grid : public imp::instance_guard<grid>
	{
		grid();

		operator bool() const;

		static bool dirty();
		static bool check_header(const view_generator& generator);
		static void add_property(std::unique_ptr<iview> prop);

		static bool check_property(std::unique_ptr<iview> prop);

		struct subproperty_scope : public imp::instance_stack<subproperty_scope>
		{
			std::vector<std::unique_ptr<iview>> subproperties;

			subproperty_scope() = default;
			subproperty_scope(const subproperty_scope&) = delete;
			subproperty_scope(subproperty_scope&&) = delete;
		};
	};

	extern void assign_reset_icon(res::icon_id icon);
}
