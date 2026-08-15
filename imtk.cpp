#include "imtk.hpp"

namespace imtk
{
	void init(const gui_config& config)
	{
		set_error_logger(config.error_logger);
		prop::assign_reset_icon(config.reset_icon);
	}
}
