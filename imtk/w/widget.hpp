#pragma once

#include "imtk/item_result.hpp"

namespace imtk::w
{
	class widget
	{
		item_result _result;

	public:
		virtual ~widget() = default;

		item_result result() const;
		item_result draw();

	protected:
		virtual item_result draw_impl() = 0;
	};
}
