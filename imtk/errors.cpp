#include "errors.hpp"

namespace imtk
{
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
		case error_code::glfw_error:
			return "glfw error";
		case error_code::existing_active_instance:
			return "existing active instance";
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
	}

	error::error(error_code ec, std::string info)
		: ec(ec), std::runtime_error("Error [" + repr(ec) + "]: " + std::string(info))
	{
	}

	void error::log() const
	{
		breakout_error::log(what());
	}
}
