#pragma once

#include "imtk/prop/view.hpp"

namespace imtk::prop::clipboard
{
	extern void clear();

	extern void store(const iview& view);
	extern bool can_paste(const iview& view);
	extern bool try_paste(const iview& view);

	extern bool context_menu_items(const iview& view);

	extern void new_session();
	extern bool context_menu(const iview& view);
	extern bool context_menu(const view_generator& generator);
}
