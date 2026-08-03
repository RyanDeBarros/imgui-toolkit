#pragma once

namespace imtk
{
	struct window_flags
	{
	private:
		int _e;

		window_flags(int e);

	public:
		window_flags();
		operator int() const;

		friend window_flags operator&(window_flags a, window_flags b);
		friend window_flags& operator&=(window_flags& a, window_flags b);
		friend window_flags operator|(window_flags a, window_flags b);
		friend window_flags& operator|=(window_flags& a, window_flags b);
		friend window_flags operator~(window_flags a);

		static window_flags none();
		static window_flags no_title_bar();
		static window_flags no_resize();
		static window_flags no_move();
		static window_flags no_scrollbar();
		static window_flags no_scroll_with_mouse();
		static window_flags no_collapse();
		static window_flags always_auto_resize();
		static window_flags no_background();
		static window_flags no_saved_settings();
		static window_flags no_mouse_inputs();
		static window_flags menu_bar();
		static window_flags horizontal_scrollbar();
		static window_flags no_focus_on_appearing();
		static window_flags no_bring_to_front_on_focus();
		static window_flags always_vertical_scrollbar();
		static window_flags always_horizontal_scrollbar();
		static window_flags no_nav_inputs();
		static window_flags no_nav_focus();
		static window_flags unsaved_document();
		static window_flags no_docking();
		static window_flags no_nav();
		static window_flags no_decoration();
		static window_flags no_inputs();
	};
}
