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
		list_indexer widget;
		list_model model;

		owned_list_indexer();
		owned_list_indexer(const owned_list_indexer&);
		owned_list_indexer(owned_list_indexer&&) noexcept;

	protected:
		item_result draw_impl() override;
	};
}
