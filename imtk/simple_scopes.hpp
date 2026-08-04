#pragma once

#include <imgui.h>

#include <string_view>
#include <vector>
#include <variant>

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

	class style_color
	{
		bool _active = false;

	public:
		style_color(ImGuiCol idx, ImU32 col);
		style_color(const style_color&) = delete;
		style_color(style_color&&) noexcept;
		~style_color();
		style_color& operator=(style_color&&) = delete;

		operator bool() const;

		void kill();
	};

	class style_var
	{
		bool _active = false;

	public:
		style_var(ImGuiStyleVar idx, float value);
		style_var(ImGuiStyleVar idx, ImVec2 value);
		style_var(const style_var&) = delete;
		style_var(style_var&&) noexcept;
		~style_var();
		style_var& operator=(style_var&&) = delete;

		operator bool() const;

		void kill();
	};

	class style_stack
	{
		struct color_ctor
		{
			ImGuiCol idx;
			ImU32 col;
		};

		struct var_1d_ctor
		{
			ImGuiStyleVar idx;
			float value;
		};

		struct var_2d_ctor
		{
			ImGuiStyleVar idx;
			ImVec2 value;
		};

		using ctor_variant = std::variant<color_ctor, var_1d_ctor, var_2d_ctor>;
		using style_variant = std::variant<style_color, style_var>;

		std::vector<ctor_variant> _ctors;

		class impl
		{
			friend style_stack;

			std::vector<style_variant> _styles;

			impl() = default;

		public:
			void kill();
		};

	public:
		impl apply();

		style_stack& push(ImGuiCol idx, ImU32 col);
		style_stack& push(ImGuiStyleVar idx, float value);
		style_stack& push(ImGuiStyleVar idx, ImVec2 value);

		void pop();
		void clear();
	};

	class style_substack
	{
		style_stack& _stack;
		size_t _count;

	public:
		style_substack(style_stack& stack);
		style_substack(const style_substack&) = delete;
		style_substack(style_substack&&) noexcept;
		~style_substack();
		style_substack& operator=(style_substack&&) = delete;

		style_substack& push(ImGuiCol idx, ImU32 col);
		style_substack& push(ImGuiStyleVar idx, float value);
		style_substack& push(ImGuiStyleVar idx, ImVec2 value);

		void pop();
		void clear();
	};

	class font_scope
	{
		bool _alive;

	public:
		font_scope(ImFont* font);
		font_scope(ImFont* font, float font_size_base_unscaled);
		font_scope(const font_scope&) = delete;
		font_scope(font_scope&&) noexcept;
		~font_scope();
		font_scope& operator=(font_scope&&) = delete;

		operator bool() const;
	};

	struct expand_item_width
	{
	};

	class item_width_scope
	{
		bool _alive;

	public:
		item_width_scope(float item_width);
		item_width_scope(expand_item_width);
		item_width_scope(const item_width_scope&) = delete;
		item_width_scope(item_width_scope&&) noexcept;
		~item_width_scope();
		item_width_scope& operator=(item_width_scope&&) = delete;

		operator bool() const;
	};
}
