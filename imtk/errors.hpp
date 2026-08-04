#pragma once

#include <stdexcept>

namespace imtk
{
	enum class error_code
	{
		none,
		init_glfw,
		init_glew,
		existing_active_instance,
		no_active_instance,
		dead_object,
		index_out_of_range,
		bad_size
	};

	struct error : public std::runtime_error
	{
		error_code ec;

		error(error_code ec);
	};
}
