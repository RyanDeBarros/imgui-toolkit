#include "errors.hpp"

namespace imtk
{
	static std::function<void(const char*)> error_logger;

	void set_error_logger(std::function<void(const char*)> logger)
	{
		error_logger = std::move(logger);
	}

	static std::string repr(error_code ec)
	{
		switch (ec)
		{
		case error_code::none:
			return "none";
		case error_code::init_glfw:
			return "init glfw";
		case error_code::init_glew:
			return "init glew";
		case error_code::existing_active_instance:
			return "existing active instance";
		case error_code::no_active_instance:
			return "no active instance";
		case error_code::dead_object:
			return "dead object";
		case error_code::load_texture:
			return "load texture";
		default:
			return "";
		}
	}

	error::error(error_code ec)
		: ec(ec), std::runtime_error("Error [" + repr(ec) + "]")
	{
		if (error_logger)
			error_logger(what());
	}

	error::error(error_code ec, std::string info)
		: ec(ec), std::runtime_error("Error [" + repr(ec) + "]: " + std::string(info))
	{
		if (error_logger)
			error_logger(what());
	}
}
