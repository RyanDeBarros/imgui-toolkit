#include "popup.hpp"

#include "imtk/errors.hpp"

namespace imtk
{
	popup::draw_impl::draw_impl(const char* name, bool modal, ImGuiWindowFlags window_flags)
		: _alive(true), _open(true)
	{
		if (modal)
			ImGui::BeginPopupModal(name, 0, window_flags);
		else
			ImGui::BeginPopup(name, window_flags);
	}

	popup::draw_impl::draw_impl(draw_impl&& o) noexcept
		: _alive(o._alive), _open(o._open)
	{
		o._alive = false;
	}

	popup::draw_impl::~draw_impl()
	{
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

	popup::popup(std::string name)
		: _name(std::move(name))
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

	popup::draw_impl popup::draw(bool modal, ImGuiWindowFlags window_flags)
	{
		if (_trigger_open)
		{
			ImGui::OpenPopup(_name.c_str());
			_trigger_open = false;
		}

		return draw_impl(_name.c_str(), modal, window_flags);
	}
}
