#include "property_grid.hpp"

#include "imtk/simple_scopes.hpp"
#include "imtk/id_scope.hpp"

#include "imtk/prop/common_views.hpp"
#include "imtk/prop/clipboard.hpp"
#include "imtk/w/widget_row.hpp"
#include "imtk/w/icon.hpp"

#include <unordered_set>

namespace imtk::prop
{
	namespace key
	{
		static std::string label_text;
		static item_state label_state;

		static void clear()
		{
			label_state = {};
			label_text.clear();
		}
	}

	namespace value
	{
		static w::widget_row components;
		static view_list properties;
		static item_result draw_result;

		static void clear()
		{
			components.subwidgets.clear();
			properties.subviews.clear();
			draw_result = {};
		}
	}

	namespace reset
	{
		static res::icon_id icon;
		static std::unordered_set<size_t> subrows_to_reset;
		static std::unordered_set<size_t> subrows_activated;

		static void clear()
		{
			subrows_to_reset.clear();
			subrows_activated.clear();
		}
	}

	namespace row
	{
		static item_result draw_result;
		static size_t row_number = 0;

		static void clear()
		{
			draw_result = {};
			row_number = 0;
		}
	}

	static bool dirty_grid = false;

	namespace key
	{
		void set_label(const std::string_view label)
		{
			label_text = label;
		}

		static void draw_cell()
		{
			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted(label_text.c_str());
			label_text.clear();

			label_state = item_state::query(); // TODO this only triggers context menu when text element is clicked - but this should actually query the full cell. Context menu should also trigger when right-clicking the value cell, not just the value items
			dirty_grid |= clipboard::context_menu(value::properties);
		}
	}

	namespace value
	{
		static bool drawing = false;

		item_result get_draw_result()
		{
			return draw_result;
		}

		void add_component(std::unique_ptr<w::widget> component)
		{
			components.subwidgets.push_back(std::move(component));
		}

		static void draw_cell()
		{
			ImGui::TableSetColumnIndex(1);
			draw_result = {};

			drawing = true;
			draw_result |= components.draw();
			drawing = false;
		}
	}

	namespace reset
	{
		void button(size_t subrow)
		{
			subrows_to_reset.insert(subrow);
		}

		bool activated(size_t subrow)
		{
			return subrows_activated.contains(subrow);
		}

		bool any_activated()
		{
			return !subrows_activated.empty();
		}

		static void draw_cell()
		{
			ImGui::TableSetColumnIndex(2);

			subrows_activated.clear();
			size_t subrow = 0;
			while (!subrows_to_reset.empty())
			{
				auto it = subrows_to_reset.find(subrow);
				if (it != subrows_to_reset.end())
				{
					subrows_to_reset.erase(it);

					id_scope scope(subrow);
					if (w::icon_button({ .icon = icon, .str_id = "##Reset", .tooltip = "Reset to default" }).draw())
						subrows_activated.insert(subrow);
				}
				else
					ImGui::NewLine();

				++subrow;
			}
		}
	}

	namespace row
	{
		item_result get_draw_result()
		{
			return draw_result;
		}

		void submit()
		{
			ImGui::TableNextRow();
			id_scope row_id(row_number++);
			reset::draw_cell();
			value::draw_cell();
			key::draw_cell();

			draw_result = value::draw_result;
			draw_result.state |= key::label_state;
			dirty_grid |= row::dirty();

			value::components.subwidgets.clear();
			value::properties.subviews.clear();
			reset::subrows_to_reset.clear();
		}

		bool dirty()
		{
			return value::get_draw_result().modified || reset::any_activated();
		}
	}

	grid::grid()
	{
		key::clear();
		value::clear();
		reset::clear();
		row::clear();

		dirty_grid = false;

		clipboard::new_session();
	}

	grid::operator bool() const
	{
		return active_instance() == this;
	}

	bool grid::dirty()
	{
		return dirty_grid;
	}

	bool grid::check_header(const view_generator& generator)
	{
		bool dirty = clipboard::context_menu(generator);
		dirty_grid |= dirty;
		return dirty;
	}

	void grid::add_property(std::unique_ptr<iview> prop)
	{
		if (value::drawing)
		{
			if (subproperty_scope::instance_count() == 0)
				value::properties.subviews.push_back(std::move(prop));
			else
				subproperty_scope::instance().subproperties.push_back(std::move(prop));
		}
	}

	bool grid::check_property(std::unique_ptr<iview> prop)
	{
		bool pasted = clipboard::context_menu(*prop);
		add_property(std::move(prop));
		return pasted;
	}

	grid::subproperty_scope::operator bool() const
	{
		return true;
	}

	void assign_reset_icon(res::icon_id icon)
	{
		reset::icon = icon;
	}
}
