#pragma once

#include <functional>
#include <stdexcept>

namespace imtk
{
	enum class error_code
	{
		none,
		init_glfw,
		init_glew,
		glfw_error,
		existing_active_instance,
		no_active_instance,
		dead_object,
		index_out_of_range,
		bad_size,
		load_texture,
	};

	extern void set_error_logger(std::function<void(const char*)> logger);

	struct error : public std::runtime_error
	{
		error_code ec;

		error(error_code ec);
		error(error_code ec, std::string info);
	};
}
