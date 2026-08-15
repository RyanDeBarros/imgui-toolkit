#pragma once

#include "external/include.hpp"
#include "imtk/include.hpp"

namespace imtk
{
	struct gui_config
	{
		std::function<void(const char*)> error_logger;
		res::icon_id reset_icon;
	};

	extern void init(const gui_config& config);
}
