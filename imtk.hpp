#pragma once

#include "external/include.hpp"
#include "imtk/include.hpp"

namespace imtk
{
	struct gui_config
	{
		std::function<void(std::string_view)> error_logger;
		res::icon_id reset_icon;
		std::string(*key_encoder)(key) = nullptr;
		key(*key_decoder)(std::string_view) = nullptr;
	};

	extern void init(const gui_config& config);
}
