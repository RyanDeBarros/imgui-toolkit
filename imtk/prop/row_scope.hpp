#pragma once

#include "imtk/prop/property_grid.hpp"

namespace imtk::prop
{
	template<typename ty>
	class row_scope
	{
		bool _valid;
		ty& _data;
		const ty& _def;

	public:
		row_scope(std::string_view label, ty& data, const ty& def)
			: _valid(true), _data(data), _def(def)
		{
			key::set_label(label);
			
			if (_data != _def)
				reset::button();
		}

		row_scope(row_scope&& o) noexcept
			: _valid(o._valid), _data(o._data), _def(o._def)
		{
			o._valid = false;
		}

		~row_scope()
		{
			if (_valid)
			{
				row::submit();
				if (reset::any_activated())
					_data = _def;
			}
		}

		row_scope(const row_scope&) = delete;

		row_scope& operator=(row_scope&&) = delete;

		operator bool() const
		{
			return _valid;
		}
	};

	template<typename ty>
	row_scope<ty> make_row_scope(std::string_view label, ty& data, const ty& def)
	{
		return row_scope<ty>(label, data, def);
	}

	template<typename ty>
	class row_scope<edit_session<ty>>
	{
		bool _valid;
		edit_session<ty>& _data;
		const ty& _def;

	public:
		row_scope(std::string_view label, edit_session<ty>& data, const ty& def)
			: _valid(true), _data(data), _def(def)
		{
			key::set_label(label);

			if (_data.buffer() != _def)
				reset::button();
		}

		row_scope(row_scope&& o) noexcept
			: _valid(o._valid), _data(o._data), _def(o._def)
		{
			o._valid = false;
		}

		~row_scope()
		{
			if (_valid)
			{
				row::submit();
				if (reset::any_activated())
					_data.publish_reset(_def);
			}
		}

		row_scope(const row_scope&) = delete;

		row_scope& operator=(row_scope&&) = delete;

		operator bool() const
		{
			return _valid;
		}
	};

	template<typename ty>
	row_scope<edit_session<ty>> make_row_scope(std::string_view label, edit_session<ty>& data, const ty& def)
	{
		return row_scope<edit_session<ty>>(label, data, def);
	}
}
