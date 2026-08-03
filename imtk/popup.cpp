#include "popup.hpp"

#include "imtk/errors.hpp"

#include <imgui.h>

namespace imtk
{
	popup::draw_impl::draw_impl(const char* name, center_window center_window, bool modal, ImGuiWindowFlags window_flags)
	{
		if (center_window == center_window::always)
			ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		else if (center_window == center_window::appearing)
			ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

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

	popup::popup(std::string name, center_window center_window, bool modal, ImGuiWindowFlags window_flags)
		: _name(std::move(name)), _center_window(center_window), _modal(modal), _window_flags(window_flags)
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

	popup::draw_impl popup::draw(std::optional<center_window> center_window_override, std::optional<bool> modal_override, enum_override<ImGuiWindowFlags> window_flags_override)
	{
		if (_trigger_open)
		{
			ImGui::OpenPopup(_name.c_str());
			_trigger_open = false;
		}

		return draw_impl(_name.c_str(), center_window_override ? *center_window_override : _center_window,
			modal_override ? *modal_override : _modal, window_flags_override.eval(_window_flags));
	}
}
