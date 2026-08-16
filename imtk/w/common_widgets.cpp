#include "common_widgets.hpp"

#include "imtk/id_scope.hpp"
#include "imtk/simple_scopes.hpp"
#include "imtk/wrapper.hpp"

#include "imtk/prop/common_views.hpp"

namespace imtk::w
{
	item_result generic_widget::draw_impl()
	{
		return draw_fn();
	}

	item_result subsequent::draw_impl()
	{
		controls::vertical_separator();
		return item->draw();
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

		result.modified |= check_property(std::make_unique<prop::combo_view>(index, names));
		return result;
	}
}
