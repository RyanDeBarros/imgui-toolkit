#pragma once

#include "imtk/w/widget.hpp"

#include <memory>
#include <span>
#include <vector>

namespace imtk::w
{
	struct widget_row : public widget
	{
		std::vector<std::unique_ptr<widget>> subwidgets;

	protected:
		item_result draw_impl();
	};
}
