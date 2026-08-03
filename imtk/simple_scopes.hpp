#pragma once

#include <imgui.h>

#include <string_view>

namespace imtk
{
	class group
	{
		bool _alive;

	public:
		group();
		group(const group&) = delete;
		group(group&&) noexcept;
		~group();
		group& operator=(group&&) = delete;

		operator bool() const;
	};

	class disabled
	{
		bool _alive;
		bool _disabled;

	public:
		disabled(bool disabled);
		disabled(const disabled&) = delete;
		disabled(disabled&&) noexcept;
		~disabled();
		disabled& operator=(disabled&&) = delete;

		operator bool() const;
		bool is_disabled() const;
	};

	class menu
	{
		bool _alive;
		bool _enabled;

	public:
		menu(const std::string_view label, bool enabled = true);
		menu(const menu&) = delete;
		menu(menu&&) noexcept;
		~menu();
		menu& operator=(menu&&) = delete;

		operator bool() const;
		bool is_enabled() const;
	};

	class menu_bar
	{
		bool _alive;

	public:
		menu_bar();
		menu_bar(const menu_bar&) = delete;
		menu_bar(menu_bar&&) noexcept;
		~menu_bar();
		menu_bar& operator=(menu_bar&&) = delete;

		operator bool() const;
	};

	class main_menu_bar
	{
		bool _alive;

	public:
		main_menu_bar();
		main_menu_bar(const main_menu_bar&) = delete;
		main_menu_bar(main_menu_bar&&) noexcept;
		~main_menu_bar();
		main_menu_bar& operator=(main_menu_bar&&) = delete;

		operator bool() const;
	};

	class tab_bar
	{
		bool _alive;

	public:
		tab_bar(const std::string_view str_id, ImGuiTabBarFlags flags = 0);
		tab_bar(const tab_bar&) = delete;
		tab_bar(tab_bar&&) noexcept;
		~tab_bar();
		tab_bar& operator=(tab_bar&&) = delete;

		operator bool() const;
	};

	class tab_item
	{
		bool _alive;

	public:
		tab_item(const std::string_view str_id, ImGuiTabItemFlags flags = 0, bool* p_open = nullptr);
		tab_item(const tab_item&) = delete;
		tab_item(tab_item&&) noexcept;
		~tab_item();
		tab_item& operator=(tab_item&&) = delete;

		operator bool() const;
	};
}
