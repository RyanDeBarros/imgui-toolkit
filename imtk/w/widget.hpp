#pragma once

#include "imtk/item_state.hpp"

namespace imtk::w
{
	class widget
	{
		item_result _result;

	public:
		virtual ~widget() = default;

		item_result result() const;
		bool draw();

	protected:
		virtual item_result draw_impl() = 0;
	};
}
