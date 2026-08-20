#include "common_widgets.hpp"

#include "imtk/id_scope.hpp"
#include "imtk/simple_scopes.hpp"
#include "imtk/controls.hpp"

#include "imtk/prop/common_views.hpp"
#include "imtk/prop/property_grid.hpp"

namespace imtk::w
{
	item_result generic_widget::draw_impl()
	{
		return draw_fn();
	}

	item_result bound_optional::draw_impl()
	{
		auto result = enable.draw();
		if (auto d = disabled(!enable.data))
		{
			ImGui::SameLine();
			result |= item->draw();
		}
		return result;
	}

	item_result simple_optional::draw_impl()
	{
		auto result = enable.draw();
		if (auto d = disabled(!enable.value))
		{
			ImGui::SameLine();
			result |= item->draw();
		}
		return result;
	}

	item_result combo_widget::draw_impl()
	{
		id_scope scope(&index);
		auto result = prefix_label(config.label);

		result |= item_result::query(ImGui::Combo("", &index, &label_span_registry::combo_getter, &names, label_span_registry::count(names)));

		result.modified |= prop::grid::check_property(std::make_unique<prop::combo_view>(index, names));
		return result;
	}

	item_result readonly_text::draw_impl()
	{
		id_scope scope(text.data());
		return prefix_label(config.label) | controls::readonly_text("", text, config.flags);
	}

	item_result readonly_text_owned::draw_impl()
	{
		return view.draw();
	}
}
