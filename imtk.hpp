#pragma once

#include "external/include.hpp"
#include "imtk/include.hpp"

namespace imtk
{
	struct gui_config
	{
		res::icon_id reset_icon;
		std::string(*key_encoder)(key) = nullptr;
		key(*key_decoder)(std::string_view) = nullptr;

		struct
		{
			res::icon_id drag_icon;
			res::icon_id create_icon;
			res::icon_id delete_icon;
			res::icon_id clear_icon;
		} dynamic_lists;

		struct
		{
			res::icon_id drag_icon;
			res::icon_id create_icon;
			res::icon_id delete_icon;
			res::icon_id clear_icon;
		} list_indexers;
	};

	extern void post_window_init(const gui_config& config);
}
