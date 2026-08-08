#include "widget.hpp"

namespace imtk::w
{
	item_state widget::state() const
	{
		return _state;
	}

	bool widget::draw()
	{
		bool modified;
		_state = {};
		draw_impl(modified, _state);
		return modified;
	}
}
