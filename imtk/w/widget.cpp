#include "widget.hpp"

#include "imtk/prop/property_grid.hpp"
#include "imtk/prop/clipboard.hpp"

namespace imtk::w
{
	void internal::add_to_grid(widget& component)
	{
		component._in_grid = true;
	}

	item_result widget::result() const
	{
		return _result;
	}

	item_result widget::draw()
	{
		_result = draw_impl();
		return _result;
	}

	bool widget::in_grid() const
	{
		return _in_grid;
	}

	bool widget::check_property(std::unique_ptr<prop::iview>&& view) const
	{
		if (_in_grid)
			return prop::value::check_property(std::move(view));
		else
			return prop::clipboard::context_menu(*view);
	}

	item_result widget::prefix_label(std::string_view label)
	{
		item_result result;

		if (!label.empty())
		{
			ImGui::TextUnformatted(label.data(), label.data() + label.size());
			result |= item_result::query(false);
			ImGui::SameLine();
		}

		return result;
	}
}
