#include "widget.hpp"

namespace imtk::w
{
	item_result widget::result() const
	{
		return _result;
	}

	item_result widget::draw()
	{
		_result = draw_impl();
		return _result;
	}
}
