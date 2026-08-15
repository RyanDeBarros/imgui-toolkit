#include "icon.hpp"

namespace imtk::w
{
	icon_image::icon_image(res::icon_id id)
		: id(id)
	{
	}

	void icon_image::draw(ImVec2 pos) const
	{
		const ImVec2 size = ImVec2(ImGui::GetFrameHeight(), ImGui::GetFrameHeight());
		const ImVec2 start = pos + 0.5f * size * padding_factor;
		const ImVec2 end = pos + size * (1.f - 0.5f * padding_factor);
		ImU32 tint = ImGui::GetColorU32(ImVec4(1.f, 1.f, 1.f, tint_alpha));
		ImGui::GetWindowDrawList()->AddImage(res::icon_texture(id).id(), start, end, ImVec2(0, 0), ImVec2(1, 1), tint);
	}

	icon_button::icon_button(icon_button_config config)
		: config(std::move(config))
	{
	}

	item_result icon_button::draw_impl()
	{
		const ImVec2 pos = ImGui::GetCursorScreenPos();
		const ImVec2 size = ImVec2(ImGui::GetFrameHeight(), ImGui::GetFrameHeight());
		auto result = imtk::item_result::query(ImGui::InvisibleButton(config.str_id, size));

		if (result)
			config.selected = !config.selected;

		if (result.state.hovered())
		{
			ImGui::GetWindowDrawList()->AddRectFilled(pos, pos + size, ImGui::GetColorU32(ImGuiCol_HeaderHovered, 0.9f), 6.0f);

			if (config.tooltip)
				ImGui::SetTooltip(config.tooltip);
		}

		if (config.selected && config.selected_icon)
			config.selected_icon->draw(pos);
		else
			config.icon.draw(pos);

		return result;
	}

	bool icon_button::selected() const
	{
		return config.selected;
	}

	icon_menu_item::icon_menu_item(icon_menu_item_config config)
		: config(std::move(config))
	{
	}

	item_result icon_menu_item::draw_impl()
	{
		auto result = imtk::item_result::query(ImGui::MenuItem(("   " + config.label).c_str()));
		config.icon.draw(ImGui::GetItemRectMin() + ImVec2(2.f, 0.f));
		return result;
	}
}
