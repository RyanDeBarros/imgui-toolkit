#include "popup.hpp"

#include "imtk/errors.hpp"

#include <imgui.h>

namespace imtk
{
	popup::draw_impl::draw_impl(const char* name, popup_config config)
	{
		if (config.center_window == center_window::always)
			ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		else if (config.center_window == center_window::appearing)
			ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (config.modal)
			_alive = ImGui::BeginPopupModal(name, 0, config.window_flags);
		else
			_alive = ImGui::BeginPopup(name, config.window_flags);

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

	popup::popup(std::string name, popup_config default_config)
		: _name(std::move(name)), _default_config(default_config)
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

	popup::draw_impl popup::draw(std::optional<popup_config> config_override)
	{
		if (_trigger_open)
		{
			ImGui::OpenPopup(_name.c_str());
			_trigger_open = false;
		}

		return draw_impl(_name.c_str(), config_override ? *config_override : _default_config);
	}
}
