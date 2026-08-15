#pragma once

#include "imtk/instance_guard.hpp"
#include "imtk/resource_loader.hpp"

#include "imtk/prop/payload.hpp"
#include "imtk/w/widget.hpp"

#include <imgui.h>

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
		extern bool check_property(std::unique_ptr<iview> prop);
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

	struct grid : public instance_guard<grid>
	{
		grid();

		operator bool() const;

		static bool dirty();
		static bool check_header(const view_generator& generator);

		// TODO begin_form()/end_form() should be internal and handled by imtk::prop::form
		static bool begin_form(ImGuiID id);
		static void end_form(bool table_visible);
	};

	extern void assign_reset_icon(res::icon_id icon);
}
