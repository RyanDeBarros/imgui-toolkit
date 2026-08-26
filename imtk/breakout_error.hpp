#pragma once

#include <stdexcept>
#include <string_view>

namespace imtk
{
	struct breakout_error : public std::exception
	{
		breakout_error(const char* message);

		[[noreturn]] static void throw_(std::string_view message);

		void log() const;
		static void log(std::string_view error);

		struct notify_scope
		{
			notify_scope(bool notify);
			notify_scope(const notify_scope&) = delete;
			notify_scope(notify_scope&&) = delete;
			~notify_scope();
		};
	};
}
