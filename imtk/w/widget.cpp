#include "widget.hpp"

#include "imtk/prop/property_grid.hpp"
#include "imtk/prop/clipboard.hpp"

namespace imtk::w
{
	item_result widget::result() const
	{
		return _result;
	}

	item_result widget::draw()
	{
		_result = draw_impl();
		return _result;
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
