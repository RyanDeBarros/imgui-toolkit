#pragma once

#include "imtk/outline.hpp"

#include <imp/event.hpp>

namespace imtk::prop
{
	class value_outline
	{
		imp::event_listener _begin_listener;
		imp::event_listener _end_listener;
		ImVec2 _begin;
		ImVec2 _end;

	public:
		value_outline();
		value_outline(const value_outline&);
		value_outline(value_outline&&) noexcept;

		value_outline& operator=(const value_outline&);
		value_outline& operator=(value_outline&&) noexcept;

		operator bool() const;

		void draw(ImU32 color, outline::config cfg = {}) const;

	private:
		void on_begin_outline();
		void on_end_outline();
	
	public:
		void kill();
	};
}
