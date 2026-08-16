#include "subform.hpp"

#include "imtk/prop/property_grid.hpp"

namespace imtk::prop
{
	// TODO v9.3 combine subform with form so that only need to call nested forms?

	// TODO DEBT use generators for as many headers as possible
	subform::subform(const char* label, const view_generator& property_generator, bool start_open)
		: _pause(), _section(label, start_open)
	{
		grid::check_header(property_generator);

		if (_section)
			_subform.emplace();
	}

	subform::subform(const char* label, bool start_open)
		: _pause(), _section(label, start_open)
	{
		if (_section)
			_subform.emplace();
	}

	subform::operator bool() const
	{
		return _pause && _section && _subform && *_subform;
	}
}
