#pragma once

#include "imtk/w/widget.hpp"
#include "imtk/w/icon.hpp"
#include "imtk/list_model.hpp"

namespace imtk::w
{
	struct list_indexer : public widget
	{
		list_model& model;
		icon_button create_button;
		icon_button delete_button;
		icon_button clear_button;
		
		std::string prompt;
		std::function<std::string(size_t)> combo_name;

		list_indexer(list_model& model);

	protected:
		item_result draw_impl() override;

	public:
		void configure_buttons(icon_image create_icon, std::string create_tooltip,
			icon_image delete_icon, std::string delete_tooltip, icon_image clear_icon, std::string clear_tooltip);
	};

	extern std::function<std::string(size_t)> make_combo_name_from_prefix(std::string slot_prefix);

	struct owned_list_indexer : public widget
	{
		list_indexer widget;
		list_model model;

		owned_list_indexer();

	protected:
		item_result draw_impl() override;
	};
}
