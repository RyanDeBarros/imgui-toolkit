#include "row_scope.hpp"

namespace imtk::prop
{
	row_scope::row_scope(std::string_view label, std::unique_ptr<iresettable> resetter)
		: _valid(true), _resetter(std::move(resetter))
	{
		key::set_label(label);
		if (_resetter->out_of_sync())
			reset::button();
	}

	row_scope::row_scope(row_scope&& o) noexcept
		: _valid(o._valid), _resetter(std::move(o._resetter))
	{
		o._valid = false;
	}

	row_scope::~row_scope()
	{
		if (_valid)
		{
			row::submit();
			if (reset::any_activated())
				_resetter->reset();
		}
	}

	row_scope::operator bool() const
	{
		return _valid;
	}
	
	multi_row_scope::multi_row_scope(std::string_view label, std::vector<std::unique_ptr<iresettable>> resetters)
		: _valid(true), _resetters(std::move(resetters))
	{
		key::set_label(label);
		for (size_t i = 0; i < _resetters.size(); ++i)
		{
			if (_resetters[i]->out_of_sync())
				reset::button(i);
		}
	}

	multi_row_scope::multi_row_scope(multi_row_scope&& o) noexcept
		: _valid(o._valid), _resetters(std::move(o._resetters))
	{
		o._valid = false;
	}

	multi_row_scope::~multi_row_scope()
	{
		if (_valid)
		{
			row::submit();
			for (size_t i = 0; i < _resetters.size(); ++i)
			{
				if (reset::activated(i))
					_resetters[i]->reset();
			}
		}
	}

	multi_row_scope::operator bool() const
	{
		return _valid;
	}
}
