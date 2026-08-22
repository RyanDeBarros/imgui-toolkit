#include "imtk.hpp"

namespace imtk
{
	static void glfw_error_callback(int err, const char* description)
	{
		std::stringstream ss;
		ss << "code " << err << ": " << description;
		throw error(error_code::glfw_error, ss.str());
	}

	void init(const gui_config& config)
	{
		set_error_logger(config.error_logger);
		prop::assign_reset_icon(config.reset_icon);
		set_key_encoder(config.key_encoder);
		set_key_decoder(config.key_decoder);
		glfwSetErrorCallback(glfw_error_callback);
	}
}
