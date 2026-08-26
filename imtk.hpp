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
	};

	extern void post_window_init(const gui_config& config);
}
