#include "flags.hpp"

#include <imgui.h>

namespace imtk
{
	window_flags::window_flags(int e)
		: _e(e)
	{
	}

	window_flags::window_flags()
		: _e(0)
	{
	}

	window_flags::operator int() const
	{
		return _e;
	}

	window_flags operator&(window_flags a, window_flags b)
	{
		return a._e & b._e;
	}

	window_flags& operator&=(window_flags& a, window_flags b)
	{
		a._e &= b._e;
		return a;
	}

	window_flags operator|(window_flags a, window_flags b)
	{
		return a._e | b._e;
	}

	window_flags& operator|=(window_flags& a, window_flags b)
	{
		a._e |= b._e;
		return a;
	}

	window_flags operator~(window_flags a)
	{
		return ~a._e;
	}

	window_flags window_flags::none()
	{
		return ImGuiWindowFlags_None;
	}

	window_flags window_flags::no_title_bar()
	{
		return ImGuiWindowFlags_NoTitleBar;
	}

	window_flags window_flags::no_resize()
	{
		return ImGuiWindowFlags_NoResize;
	}

	window_flags window_flags::no_move()
	{
		return ImGuiWindowFlags_NoMove;
	}

	window_flags window_flags::no_scrollbar()
	{
		return ImGuiWindowFlags_NoScrollbar;
	}

	window_flags window_flags::no_scroll_with_mouse()
	{
		return ImGuiWindowFlags_NoScrollWithMouse;
	}

	window_flags window_flags::no_collapse()
	{
		return ImGuiWindowFlags_NoCollapse;
	}

	window_flags window_flags::always_auto_resize()
	{
		return ImGuiWindowFlags_AlwaysAutoResize;
	}

	window_flags window_flags::no_background()
	{
		return ImGuiWindowFlags_NoBackground;
	}

	window_flags window_flags::no_saved_settings()
	{
		return ImGuiWindowFlags_NoSavedSettings;
	}

	window_flags window_flags::no_mouse_inputs()
	{
		return ImGuiWindowFlags_NoMouseInputs;
	}

	window_flags window_flags::menu_bar()
	{
		return ImGuiWindowFlags_MenuBar;
	}

	window_flags window_flags::horizontal_scrollbar()
	{
		return ImGuiWindowFlags_HorizontalScrollbar;
	}

	window_flags window_flags::no_focus_on_appearing()
	{
		return ImGuiWindowFlags_NoFocusOnAppearing;
	}

	window_flags window_flags::no_bring_to_front_on_focus()
	{
		return ImGuiWindowFlags_NoBringToFrontOnFocus;
	}

	window_flags window_flags::always_vertical_scrollbar()
	{
		return ImGuiWindowFlags_AlwaysVerticalScrollbar;
	}

	window_flags window_flags::always_horizontal_scrollbar()
	{
		return ImGuiWindowFlags_AlwaysHorizontalScrollbar;
	}

	window_flags window_flags::no_nav_inputs()
	{
		return ImGuiWindowFlags_NoNavInputs;
	}

	window_flags window_flags::no_nav_focus()
	{
		return ImGuiWindowFlags_NoNavFocus;
	}

	window_flags window_flags::unsaved_document()
	{
		return ImGuiWindowFlags_UnsavedDocument;
	}

	window_flags window_flags::no_docking()
	{
		return ImGuiWindowFlags_NoDocking;
	}

	window_flags window_flags::no_nav()
	{
		return ImGuiWindowFlags_NoNav;
	}

	window_flags window_flags::no_decoration()
	{
		return ImGuiWindowFlags_NoDecoration;
	}

	window_flags window_flags::no_inputs()
	{
		return ImGuiWindowFlags_NoInputs;
	}
}
