#pragma once

#include "imtk/w/widget.hpp"
#include "imtk/resource_loader.hpp"

#include <imgui.h>

#include <optional>
#include <string>

namespace imtk::w
{
	struct icon_image
	{
		res::icon_id id;
		float padding_factor = 0.f;
		float tint_alpha = 1.f;

		icon_image() = default;
		icon_image(res::icon_id id);

		void draw(ImVec2 pos) const;
	};

	struct icon_button_config
	{
		icon_image icon;
		const char* str_id = "";
		bool selected = false;
		const char* tooltip = "";
		std::optional<icon_image> selected_icon;
	};

	struct icon_button : public widget
	{
		icon_button_config config;

		icon_button(icon_button_config config = {});

		bool selected() const;

	protected:
		item_result draw_impl() override;
	};

	struct icon_menu_item_config
	{
		icon_image icon;
		std::string label;
	};

	struct icon_menu_item : public widget
	{
		icon_menu_item_config config;

		icon_menu_item(icon_menu_item_config config = {});

	protected:
		item_result draw_impl() override;
	};
}
