#pragma once

#include "imtk/item_result.hpp"

#include "imtk/prop/payload.hpp"

namespace imtk::w
{
	class widget;

	namespace internal
	{
		extern void add_to_grid(widget& component);
	}

	class widget
	{
		item_result _result;
		
		friend void internal::add_to_grid(widget& component);
		bool _in_grid = false;

	public:
		virtual ~widget() = default;

		item_result result() const;
		item_result draw();
		bool in_grid() const;

	protected:
		virtual item_result draw_impl() = 0;
		bool check_property(std::unique_ptr<prop::iview>&& view) const;
		static item_result prefix_label(std::string_view label);
	};
}
