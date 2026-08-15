#include "item_state.hpp"

#include <imgui.h>
#include <imgui_internal.h>

namespace imtk
{
	enum flags
	{
		none = 0,
		hovered = 1 << 0,
		left_clicked = 1 << 1,
		right_clicked = 1 << 2,
		middle_clicked = 1 << 3,
		focused = 1 << 4,
		active = 1 << 5,
		activated = 1 << 6,
		deactivated = 1 << 7,
		deactivated_after_edit = 1 << 8,
		edited = 1 << 9,
		visible = 1 << 10,
		toggled_open = 1 << 11,
		toggled_selection = 1 << 12
	};

	item_state item_state::query()
	{
		item_state state;

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			state._flags |= flags::hovered;

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			state._flags |= flags::left_clicked;

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			state._flags |= flags::right_clicked;

		if (ImGui::IsItemClicked(ImGuiMouseButton_Middle))
			state._flags |= flags::middle_clicked;

		if (ImGui::IsItemFocused())
			state._flags |= flags::focused;

		if (ImGui::IsItemActive())
			state._flags |= flags::active;

		if (ImGui::IsItemActivated())
			state._flags |= flags::activated;
		
		if (ImGui::IsItemDeactivated())
			state._flags |= flags::deactivated;

		if (ImGui::IsItemDeactivatedAfterEdit())
			state._flags |= flags::deactivated_after_edit;

		if (ImGui::IsItemEdited())
			state._flags |= flags::edited;

		if (ImGui::IsItemVisible())
			state._flags |= flags::visible;

		if (ImGui::IsItemToggledOpen())
			state._flags |= flags::toggled_open;

		if (ImGui::GetCurrentContext()->CurrentMultiSelect && ImGui::IsItemToggledSelection())
			state._flags |= flags::toggled_selection;

		return state;
	}

	item_state item_state::operator|(item_state o) const
	{
		item_state result;
		result._flags = _flags | o._flags;
		return result;
	}

	item_state& item_state::operator|=(const item_state& o)
	{
		_flags |= o._flags;
		return *this;
	}

	bool item_state::hovered() const
	{
		return _flags & flags::hovered;
	}
	
	bool item_state::left_clicked() const
	{
		return _flags & flags::left_clicked;
	}
	
	bool item_state::right_clicked() const
	{
		return _flags & flags::right_clicked;
	}
	
	bool item_state::middle_clicked() const
	{
		return _flags & flags::middle_clicked;
	}
	
	bool item_state::clicked() const
	{
		return _flags & (flags::left_clicked | flags::right_clicked | flags::middle_clicked);
	}
	
	bool item_state::focused() const
	{
		return _flags & flags::focused;
	}
	
	bool item_state::active() const
	{
		return _flags & flags::active;
	}
	
	bool item_state::activated() const
	{
		return _flags & flags::activated;
	}
	
	bool item_state::deactivated() const
	{
		return _flags & flags::deactivated;
	}
	
	bool item_state::deactivated_after_edit() const
	{
		return _flags & flags::deactivated_after_edit;
	}
	
	bool item_state::edited() const
	{
		return _flags & flags::edited;
	}
	
	bool item_state::visible() const
	{
		return _flags & flags::visible;
	}
	
	bool item_state::toggled_open() const
	{
		return _flags & flags::toggled_open;
	}
	
	bool item_state::toggled_selection() const
	{
		return _flags & flags::toggled_selection;
	}

	item_result item_result::query(bool modified)
	{
		return { .modified = modified, .state = item_state::query() };
	}

	item_result item_result::operator|(item_result o) const
	{
		return { .modified = modified || o.modified, .state = state | o.state };
	}

	item_result& item_result::operator|=(const item_result& o)
	{
		modified |= o.modified;
		state |= o.state;
		return *this;
	}

	item_result::operator bool() const
	{
		return modified;
	}
}
