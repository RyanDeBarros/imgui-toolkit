#include "simple_scopes.hpp"

namespace imtk
{
	group::group()
	{
		ImGui::BeginGroup();
		_alive = true;
	}

	group::group(group&& o) noexcept
		: _alive(o._alive)
	{
		o._alive = false;
	}
	
	group::~group()
	{
		if (_alive)
			ImGui::EndGroup();
	}

	group::operator bool() const
	{
		return _alive;
	}

	disabled::disabled(bool disabled)
		: _disabled(disabled)
	{
		ImGui::BeginDisabled(disabled);
		_alive = true;
	}

	disabled::disabled(disabled&& o) noexcept
		: _alive(o._alive), _disabled(o._disabled)
	{
		o._alive = false;
	}

	disabled::~disabled()
	{
		if (_alive)
			ImGui::EndDisabled();
	}

	disabled::operator bool() const
	{
		return _alive;
	}

	bool disabled::is_disabled() const
	{
		return _disabled;
	}

	menu::menu(const std::string_view label, bool enabled)
		: _enabled(enabled)
	{
		_alive = ImGui::BeginMenu(label.data(), enabled);
	}

	menu::menu(menu&& o) noexcept
		: _alive(o._alive), _enabled(o._enabled)
	{
		o._alive = false;
	}

	menu::~menu()
	{
		if (_alive)
			ImGui::EndMenu();
	}

	menu::operator bool() const
	{
		return _alive;
	}

	bool menu::is_enabled() const
	{
		return _enabled;
	}

	menu_bar::menu_bar()
	{
		_alive = ImGui::BeginMenuBar();
	}

	menu_bar::menu_bar(menu_bar&& o) noexcept
		: _alive(o._alive)
	{
		o._alive = false;
	}

	menu_bar::~menu_bar()
	{
		if (_alive)
			ImGui::EndMenuBar();
	}

	menu_bar::operator bool() const
	{
		return _alive;
	}

	main_menu_bar::main_menu_bar()
	{
		_alive = ImGui::BeginMainMenuBar();
	}
	
	main_menu_bar::main_menu_bar(main_menu_bar&& o) noexcept
		: _alive(o._alive)
	{
		o._alive = false;
	}

	main_menu_bar::~main_menu_bar()
	{
		if (_alive)
			ImGui::EndMainMenuBar();
	}

	main_menu_bar::operator bool() const
	{
		return _alive;
	}

	tab_bar::tab_bar(const std::string_view str_id, ImGuiTabBarFlags flags)
	{
		_alive = ImGui::BeginTabBar(str_id.data(), flags);
	}

	tab_bar::tab_bar(tab_bar&& o) noexcept
		: _alive(o._alive)
	{
		o._alive = false;
	}

	tab_bar::~tab_bar()
	{
		if (_alive)
			ImGui::EndTabBar();
	}

	tab_bar::operator bool() const
	{
		return _alive;
	}

	tab_item::tab_item(const std::string_view str_id, ImGuiTabItemFlags flags, bool* p_open)
	{
		_alive = ImGui::BeginTabItem(str_id.data(), p_open, flags);
	}

	tab_item::tab_item(tab_item&& o) noexcept
		: _alive(o._alive)
	{
		o._alive = false;
	}

	tab_item::~tab_item()
	{
		if (_alive)
			ImGui::EndTabItem();
	}

	tab_item::operator bool() const
	{
		return _alive;
	}
}
