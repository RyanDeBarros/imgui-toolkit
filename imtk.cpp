#include "imtk.hpp"

namespace imtk
{
	static void glfw_error_callback(int err, const char* description)
	{
		std::stringstream ss;
		ss << "code " << err << ": " << description;
		throw error(error_code::glfw_error, ss.str());
	}

	void post_window_init(const gui_config& config)
	{
		prop::assign_reset_icon(config.reset_icon);
		w::assign_dynamic_list_icons(config.dynamic_lists.drag_icon, config.dynamic_lists.create_icon, config.dynamic_lists.delete_icon, config.dynamic_lists.clear_icon);
		set_key_encoder(config.key_encoder);
		set_key_decoder(config.key_decoder);
		glfwSetErrorCallback(glfw_error_callback);
	}
}
