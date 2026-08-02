#include "popup.hpp"

#include <imgui.h>

namespace imtk
{
	popup::draw_impl::draw_impl(const char* name, bool modal)
		: _alive(true)
	{
		if (modal)
			ImGui::BeginPopupModal(name);
		else
			ImGui::BeginPopup(name);
	}

	popup::draw_impl::draw_impl(draw_impl&& o) noexcept
		: _alive(o._alive)
	{
		o._alive = false;
	}

	popup::draw_impl::~draw_impl()
	{
		ImGui::EndPopup();
	}

	void popup::draw_impl::close()
	{
		ImGui::CloseCurrentPopup();
	}

	popup::popup(std::string name)
		: _name(std::move(name))
	{
	}

	void popup::open()
	{
		_trigger_open = true;
	}

	popup::draw_impl popup::draw(bool modal)
	{
		if (_trigger_open)
		{
			ImGui::OpenPopup(_name.c_str());
			_trigger_open = false;
		}

		return draw_impl(_name.c_str(), modal);
	}
}
