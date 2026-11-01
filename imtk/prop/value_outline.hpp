#pragma once

#include "imtk/outline.hpp"

#include <imp/event.hpp>

namespace imtk::prop
{
	class value_outline
	{
		imp::event_listener begin_listener_;
		imp::event_listener end_listener_;
		ImVec2 begin_;
		ImVec2 end_;

	public:
		value_outline();
		value_outline(const value_outline&);
		value_outline(value_outline&&) noexcept;

		value_outline& operator=(const value_outline&);
		value_outline& operator=(value_outline&&) noexcept;

		operator bool() const;

    public:
		void draw(ImU32 color, outline::config cfg = {}) const;
		void kill();

    private:
        void on_begin_outline();
        void on_end_outline();
	};
}
