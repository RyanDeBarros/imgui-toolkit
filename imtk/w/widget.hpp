#pragma once

#include "imtk/item_state.hpp"

namespace imtk::w
{
	class widget
	{
		item_state _state;
		bool _modified = false;

	public:
		virtual ~widget() = default;

		item_state state() const;
		bool modified() const;
		bool draw();

	protected:
		virtual void draw_impl(bool& modified, item_state& state) = 0;
	};
}
