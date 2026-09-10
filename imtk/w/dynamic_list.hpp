#pragma once

#include "imtk/w/icon.hpp"
#include "imtk/w/widget.hpp"

#include "imtk/list_model.hpp"
#include "imtk/simple_scopes.hpp"

namespace imtk
{
	namespace w
	{
		struct dynamic_list_header : public widget
		{
			list_model& model;
			icon_button create_button;
			icon_button delete_button;
			icon_button clear_button;

			dynamic_list_header(list_model& model);
			dynamic_list_header(list_model& model, const dynamic_list_header& o);
			dynamic_list_header(list_model& model, dynamic_list_header&& o) noexcept;

			dynamic_list_header& operator=(const dynamic_list_header& o);
			dynamic_list_header& operator=(dynamic_list_header&& o) noexcept;

		protected:
			item_result draw_impl() override;
		};
	}

	class dynamic_row
	{
		list_model& _model;
		w::icon_button& _drag_button;
		bool _visible = false;
		ImVec2 _cursor, _size;
		size_t _index;
		std::unique_ptr<child> _child;

	public:
		dynamic_row(list_model& model, w::icon_button& _drag_button, size_t index, const char* str_id);
		dynamic_row(const dynamic_row&) = delete;
		dynamic_row(dynamic_row&&) = delete;
		~dynamic_row();

		operator bool() const;

		void on_select();
		size_t index() const;
		ImVec2 size() const;
	};

	namespace w
	{
		struct dynamic_list_body : public widget
		{
			list_model& model;
			std::function<item_result(dynamic_row&)> row_draw;
			icon_button drag_button;

			dynamic_list_body(list_model& model);
			dynamic_list_body(list_model& model, const dynamic_list_body& o);
			dynamic_list_body(list_model& model, dynamic_list_body&& o) noexcept;

			dynamic_list_body& operator=(const dynamic_list_body& o);
			dynamic_list_body& operator=(dynamic_list_body&& o) noexcept;

		protected:
			item_result draw_impl() override;
		};
	}

	struct dynamic_list
	{
		list_model model;
		w::dynamic_list_header header;
		w::dynamic_list_body body;

		dynamic_list();
		dynamic_list(const dynamic_list& o);
		dynamic_list(dynamic_list&& o) noexcept;

		dynamic_list& operator=(const dynamic_list&) = default;
		dynamic_list& operator=(dynamic_list&&) noexcept = default;

		item_result draw(size_t list_size);
	};

	extern void assign_dynamic_list_icons(res::icon_id drag_icon, res::icon_id create_icon, res::icon_id delete_icon, res::icon_id clear_icon);

	namespace w
	{
		struct bound_dynamic_list : public widget
		{
			dynamic_list& list;
			size_t list_size = 0;

			bound_dynamic_list(dynamic_list& list, size_t list_size);

		protected:
			item_result draw_impl() override;
		};
	}
}
