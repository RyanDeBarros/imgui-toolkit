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

		struct config
		{
			std::string prompt;
			std::string create_tooltip = "New";
			std::string delete_tooltip = "Delete";
			std::string clear_tooltip = "Clear";
		};

		list_indexer(list_model& model);
		list_indexer(list_model& model, config cfg, std::function<std::string(size_t)> combo_name);
		list_indexer(list_model& model, config cfg, std::string combo_slot_prefix);
		list_indexer(list_model& model, const list_indexer& o);
		list_indexer(list_model& model, list_indexer&& o) noexcept;
		list_indexer& operator=(const list_indexer& o);
		list_indexer& operator=(list_indexer&& o) noexcept;

	protected:
		item_result draw_impl() override;
	};

	extern std::function<std::string(size_t)> make_combo_name_from_prefix(std::string slot_prefix);

	struct owned_list_indexer : public widget
	{
		list_model model;
		list_indexer widget;

		owned_list_indexer();
		owned_list_indexer(list_indexer::config cfg, std::function<std::string(size_t)> combo_name);
		owned_list_indexer(list_indexer::config cfg, std::string combo_slot_prefix);
		owned_list_indexer(const owned_list_indexer&);
		owned_list_indexer(owned_list_indexer&&) noexcept;

	protected:
		item_result draw_impl() override;
	};

	extern void assign_list_indexer_icons(res::icon_id create_icon, res::icon_id delete_icon, res::icon_id clear_icon);
}
