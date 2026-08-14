#include "widget.hpp"

namespace imtk::w
{
	item_state widget::state() const
	{
		return _state;
	}

	bool widget::modified() const
	{
		return _modified;
	}

	bool widget::draw()
	{
		_modified = false;
		_state = {};
		draw_impl(_modified, _state);
		return _modified;
	}
}
