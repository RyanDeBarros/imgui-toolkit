#include "common_widgets.hpp"

#include <imgui.h>

namespace imtk::w
{
	generic_widget::generic_widget(std::function<item_result()> draw_fn)
		: draw_fn(std::move(draw_fn))
	{
	}

	item_result generic_widget::draw_impl()
	{
		return draw_fn();
	}

	item_result text::draw_impl()
	{
		ImGui::TextUnformatted(text.c_str());
		return item_result::query(false);
	}

	item_result text_view::draw_impl()
	{
		ImGui::TextUnformatted(text.data(), text.data() + text.size());
		return item_result::query(false);
	}
}
