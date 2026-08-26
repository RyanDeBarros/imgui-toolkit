#pragma once

#include <string>
#include <vector>

#include <imgui.h>

#include <imp/soft_singleton.hpp>

namespace imtk
{
	enum class log_level
	{
		info,
		success,
		warning,
		error
	};

	extern ImU32 log_level_color(log_level level);
	extern const char* log_level_prefix(log_level level);

	struct log_entry
	{
		log_level level;
		std::string msg;
	};

	extern void log(log_level level, std::string msg);
	extern void log_info(std::string msg);
	extern void log_success(std::string msg);
	extern void log_warning(std::string msg);
	extern void log_error(std::string msg);

	extern void clear_log();
	extern const std::vector<log_entry>& log_entries();
}
