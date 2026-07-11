#pragma once

#include <stdexcept>

namespace imtk
{
	enum class error_code
	{
		none,
		init_glfw,
		init_glew,
	};

	struct error : public std::runtime_error
	{
		error_code ec;

		error(error_code ec)
			: ec(ec)
		{
		}
	};
}
