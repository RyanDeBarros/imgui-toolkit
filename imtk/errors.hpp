#pragma once

#include "imtk/breakout_error.hpp"

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

	struct error : public std::runtime_error
	{
		error_code ec;

		error(error_code ec);
		error(error_code ec, std::string info);

		void log() const;
	};

	template<typename func>
	void handle_errors(func&& func_)
	{
		imp::handle_error<error, breakout_error>(std::forward<func>(func_), [](const auto& e) { e.log(); });
	}
}
