#pragma once

#include <functional>
#include <stdexcept>

#include <imp/error_logging.hpp>

namespace imtk
{
	enum class error_code
	{
		none,
		init_glfw,
		init_glew,
		glfw_error,
		existing_active_instance,
		dead_object,
		index_out_of_range,
		bad_size,
		load_texture,
	};

	extern void set_error_logger(std::function<void(std::string_view)> logger);
	extern void log_error(std::string_view message);

	struct error : public std::runtime_error
	{
		error_code ec;

		error(error_code ec);
		error(error_code ec, std::string info);
	};

	template<typename func>
	void handle_error(func&& func_)
	{
		imp::handle_error<error>(std::forward<func>(func_), [](const error& e) { log_error(e.what()); });
	}
}
