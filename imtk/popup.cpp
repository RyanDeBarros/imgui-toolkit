#include "popup.hpp"

#include "imtk/errors.hpp"

#include <imgui.h>

namespace imtk
{
	popup::draw_impl::draw_impl(const char* name, bool modal, window_flags window_flags)
	{
		if (modal)
			_alive = ImGui::BeginPopupModal(name, 0, window_flags);
		else
			_alive = ImGui::BeginPopup(name, window_flags);

		_open = _alive;
	}

	popup::draw_impl::draw_impl(draw_impl&& o) noexcept
		: _alive(o._alive), _open(o._open)
	{
		o._alive = false;
	}

	popup::draw_impl::~draw_impl()
	{
		if (_alive)
			ImGui::EndPopup();
	}

	void popup::draw_impl::close()
	{
		if (_alive)
		{
			ImGui::CloseCurrentPopup();
			_open = false;
		}
		else
			throw error(error_code::dead_object);
	}

	popup::draw_impl::operator bool() const
	{
		return _alive && _open;
	}

	popup::popup(std::string name, bool modal, window_flags window_flags)
		: _name(std::move(name)), _modal(modal), _window_flags(window_flags)
	{
	}

	void popup::open()
	{
		_trigger_open = true;
	}

	bool popup::is_opening() const
	{
		return _trigger_open;
	}

	popup::draw_impl popup::draw(std::optional<bool> modal_override, enum_override<window_flags> window_flags_override)
	{
		if (_trigger_open)
		{
			ImGui::OpenPopup(_name.c_str());
			_trigger_open = false;
		}

		return draw_impl(_name.c_str(), modal_override ? *modal_override : _modal, window_flags_override.eval(_window_flags));
	}
}
