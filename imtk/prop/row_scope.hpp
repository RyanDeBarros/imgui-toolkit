#pragma once

#include "imtk/prop/property_grid.hpp"

#include <imp/group.hpp>

namespace imtk::prop
{
	template<typename ty, typename def_ty>
	class row_scope
	{
		bool _valid;
		ty& _data;
		const def_ty& _def;

	public:
		row_scope(std::string_view label, ty& data, const def_ty& def)
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

	template<typename ty, typename def_ty>
	row_scope<ty, def_ty> make_row_scope(std::string_view label, ty& data, const def_ty& def)
	{
		return row_scope<ty, def_ty>(label, data, def);
	}

	template<typename ty, typename def_ty>
	class row_scope<edit_session<ty>, def_ty>
	{
		bool _valid;
		edit_session<ty>& _data;
		const def_ty& _def;

	public:
		row_scope(std::string_view label, edit_session<ty>& data, const def_ty& def)
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

	template<typename ty, typename def_ty>
	row_scope<edit_session<ty>, def_ty> make_row_scope(std::string_view label, edit_session<ty>& data, const def_ty& def)
	{
		return row_scope<edit_session<ty>, def_ty>(label, data, def);
	}
}
