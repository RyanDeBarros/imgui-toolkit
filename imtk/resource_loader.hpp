#pragma once

#include "imtk/texture.hpp"

#include <string_view>

namespace imtk::res
{
	struct icon_id
	{
		size_t id = 0;

		operator bool() const;
	};

	extern icon_id load_icon(std::string_view filepath);
	extern texture icon_texture(icon_id id);
}
