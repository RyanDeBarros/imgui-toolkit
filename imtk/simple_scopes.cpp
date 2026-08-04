#include "simple_scopes.hpp"

#include "imtk/errors.hpp"

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

	style_color::style_color(ImGuiCol idx, ImU32 col)
	{
		ImGui::PushStyleColor(idx, col);
		_active = true;
	}

	style_color::style_color(style_color&& o) noexcept
		: _active(o._active)
	{
		o._active = false;
	}

	style_color::~style_color()
	{
		kill();
	}

	style_color::operator bool() const
	{
		return _active;
	}

	void style_color::kill()
	{
		if (_active)
		{
			ImGui::PopStyleColor();
			_active = false;
		}
	}

	style_var::style_var(ImGuiStyleVar idx, float value)
	{
		ImGui::PushStyleVar(idx, value);
		_active = true;
	}

	style_var::style_var(ImGuiStyleVar idx, ImVec2 value)
	{
		ImGui::PushStyleVar(idx, value);
		_active = true;
	}

	style_var::style_var(style_var&& o) noexcept
		: _active(o._active)
	{
		o._active = false;
	}

	style_var::~style_var()
	{
		kill();
	}

	style_var::operator bool() const
	{
		return _active;
	}

	void style_var::kill()
	{
		if (_active)
		{
			ImGui::PopStyleVar();
			_active = false;
		}
	}

	void style_stack::impl::kill()
	{
		_styles.clear();
	}

	style_stack::impl style_stack::apply()
	{
		std::vector<style_variant> styles;
		styles.reserve(_ctors.size());
		for (const auto& ctor : _ctors)
		{
			std::visit([&styles](auto&& ctor) {
				using C = std::decay_t<decltype(ctor)>;
				if constexpr (std::is_same_v<C, color_ctor>)
					styles.push_back(style_color(ctor.idx, ctor.col));

				if constexpr (std::is_same_v<C, var_1d_ctor>)
					styles.push_back(style_var(ctor.idx, ctor.value));

				if constexpr (std::is_same_v<C, var_2d_ctor>)
					styles.push_back(style_var(ctor.idx, ctor.value));
			}, ctor);
		}

		impl i;
		i._styles = std::move(styles);
		return i;
	}

	style_stack& style_stack::push(ImGuiCol idx, ImU32 col)
	{
		_ctors.push_back(color_ctor{ .idx = idx, .col = col });
		return *this;
	}

	style_stack& style_stack::push(ImGuiStyleVar idx, float value)
	{
		_ctors.push_back(var_1d_ctor{ .idx = idx, .value = value });
		return *this;
	}

	style_stack& style_stack::push(ImGuiStyleVar idx, ImVec2 value)
	{
		_ctors.push_back(var_2d_ctor{ .idx = idx, .value = value });
		return *this;
	}

	void style_stack::pop()
	{
		_ctors.pop_back();
	}

	void style_stack::clear()
	{
		_ctors.clear();
	}

	style_substack::style_substack(style_stack& stack)
		: _stack(stack), _count(0)
	{
	}

	style_substack::style_substack(style_substack&& o) noexcept
		: _stack(o._stack), _count(o._count)
	{
		o._count = 0;
	}

	style_substack::~style_substack()
	{
		clear();
	}
	
	style_substack& style_substack::push(ImGuiCol idx, ImU32 col)
	{
		_stack.push(idx, col);
		++_count;
		return *this;
	}

	style_substack& style_substack::push(ImGuiStyleVar idx, float value)
	{
		_stack.push(idx, value);
		++_count;
		return *this;
	}

	style_substack& style_substack::push(ImGuiStyleVar idx, ImVec2 value)
	{
		_stack.push(idx, value);
		++_count;
		return *this;
	}

	void style_substack::pop()
	{
		if (_count > 0)
		{
			_stack.pop();
			--_count;
		}
		else
			throw error(error_code::bad_size);
	}

	void style_substack::clear()
	{
		while (_count > 0)
		{
			_stack.pop();
			--_count;
		}
	}

	font_scope::font_scope(ImFont* font)
	{
		ImGui::PushFont(font);
		_alive = true;
	}

	font_scope::font_scope(ImFont* font, float font_size_base_unscaled)
	{
		ImGui::PushFont(font, font_size_base_unscaled);
		_alive = true;
	}

	font_scope::font_scope(font_scope&& o) noexcept
		: _alive(o._alive)
	{
		o._alive = false;
	}

	font_scope::~font_scope()
	{
		if (_alive)
			ImGui::PopFont();
	}

	font_scope::operator bool() const
	{
		return _alive;
	}

	item_width_scope::item_width_scope(float item_width)
	{
		ImGui::PushItemWidth(item_width);
		_alive = true;
	}

	item_width_scope::item_width_scope(expand_item_width)
	{
		ImGui::PushItemWidth(-FLT_MIN);
		_alive = true;
	}

	item_width_scope::item_width_scope(item_width_scope&& o) noexcept
		: _alive(o._alive)
	{
		o._alive = false;
	}

	item_width_scope::~item_width_scope()
	{
		if (_alive)
			ImGui::PopItemWidth();
	}

	item_width_scope::operator bool() const
	{
		return _alive;
	}

	context_menu::context_menu(target target, const std::string_view str_id, ImGuiPopupFlags flags)
	{
		switch (target)
		{
		case target::window:
			_alive = ImGui::BeginPopupContextWindow(str_id.data(), flags);
			break;

		case target::item:
			_alive = ImGui::BeginPopupContextItem(str_id.data(), flags);
			break;

		case target::nothing:
			_alive = ImGui::BeginPopupContextVoid(str_id.data(), flags);
			break;

		default:
			_alive = false;
		}
	}

	context_menu::context_menu(context_menu&& o) noexcept
		: _alive(o._alive)
	{
		o._alive = false;
	}

	context_menu::~context_menu()
	{
		if (_alive)
			ImGui::EndPopup();
	}

	context_menu context_menu::window(const std::string_view str_id, ImGuiPopupFlags flags)
	{
		return context_menu(target::window, str_id, flags);
	}

	context_menu context_menu::item(const std::string_view str_id, ImGuiPopupFlags flags)
	{
		return context_menu(target::item, str_id, flags);
	}

	context_menu context_menu::nothing(const std::string_view str_id, ImGuiPopupFlags flags)
	{
		return context_menu(target::nothing, str_id, flags);
	}

	context_menu::operator bool() const
	{
		return _alive;
	}

	window::window(const std::string_view name, ImGuiWindowFlags flags, bool* p_open)
	{
		_visible = ImGui::Begin(name.data(), p_open, flags);
		_alive = true;
	}

	window::window(window&& o) noexcept
		: _alive(o._alive), _visible(o._visible)
	{
		o._alive = false;
	}

	window::~window()
	{
		if (_alive)
			ImGui::End();
	}

	window::operator bool() const
	{
		return _alive && _visible;
	}

	child::child(const std::string_view str_id, ImVec2 size, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags)
	{
		_visible = ImGui::BeginChild(str_id.data(), size, child_flags, window_flags);
		_alive = true;
	}

	child::child(ImGuiID id, ImVec2 size, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags)
	{
		_visible = ImGui::BeginChild(id, size, child_flags, window_flags);
		_alive = true;
	}

	child::child(child&& o) noexcept
		: _alive(o._alive), _visible(o._visible)
	{
		o._alive = false;
	}

	child::~child()
	{
		if (_alive)
			ImGui::EndChild();
	}

	child::operator bool() const
	{
		return _alive && _visible;
	}

	drag_drop_source::drag_drop_source(ImGuiDragDropFlags flags)
	{
		_alive = ImGui::BeginDragDropSource(flags);
	}

	drag_drop_source::drag_drop_source(drag_drop_source&& o) noexcept
		: _alive(o._alive)
	{
		o._alive = false;
	}
	
	drag_drop_source::~drag_drop_source()
	{
		if (_alive)
			ImGui::EndDragDropSource();
	}

	drag_drop_source::operator bool() const
	{
		return _alive;
	}

	drag_drop_target::drag_drop_target()
	{
		_alive = ImGui::BeginDragDropTarget();
	}

	drag_drop_target::drag_drop_target(drag_drop_target&& o) noexcept
		: _alive(o._alive)
	{
		o._alive = false;
	}

	drag_drop_target::~drag_drop_target()
	{
		if (_alive)
			ImGui::EndDragDropTarget();
	}

	drag_drop_target::operator bool() const
	{
		return _alive;
	}
}
