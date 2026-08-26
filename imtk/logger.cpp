#include "logger.hpp"

#include "imtk/color.hpp"

namespace imtk
{
	std::vector<log_entry> log_lines;

    ImU32 log_level_color(log_level level)
    {
        switch (level)
        {
        case log_level::success:
            return imtk::col::success;
        case log_level::warning:
            return imtk::col::warning;
        case log_level::error:
            return imtk::col::error;
        default:
            return imtk::col::white;
        }
    }

    const char* log_level_prefix(log_level level)
    {
        switch (level)
        {
        case log_level::info:
            return "[info]";
        case log_level::success:
            return "[success]";
        case log_level::warning:
            return "[warning]";
        case log_level::error:
            return "[error]";
        default:
            return "";
        }
    }

    void log(log_level level, std::string msg)
    {
        log_lines.emplace_back(level, std::move(msg));
    }

    void log_info(std::string msg)
    {
        log(log_level::info, std::move(msg));
    }

    void log_success(std::string msg)
    {
        log(log_level::success, std::move(msg));
    }

    void log_warning(std::string msg)
    {
        log(log_level::warning, std::move(msg));
    }

    void log_error(std::string msg)
    {
        log(log_level::error, std::move(msg));
    }

    void clear_log()
    {
        return log_lines.clear();
    }

    const std::vector<log_entry>& log_entries()
    {
        return log_lines;
    }
}
