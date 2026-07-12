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
			return "init_glfw";
		case error_code::init_glew:
			return "init_glew";
		default:
			return "";
		}
	}

	error::error(error_code ec)
		: ec(ec), std::runtime_error("Error [" + repr(ec) + "]")
	{
	}
}
