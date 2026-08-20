#pragma once

#include "imtk/item_result.hpp"

#include <string_view>

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
		static item_result prefix_label(std::string_view label);
	};
}
