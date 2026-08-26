#include "breakout_error.hpp"

#include "imtk/logger.hpp"
#include "imtk/notifier.hpp"

#include <stack>

namespace imtk
{
	static std::stack<bool> notify_stack;

	breakout_error::breakout_error(const char* message)
		: std::exception(message)
	{
	}

	void breakout_error::throw_(std::string_view message)
	{
		throw breakout_error(message.data());
	}

	void breakout_error::log() const
	{
		log(what());
	}

	void breakout_error::log(std::string_view error)
	{
		if (!notify_stack.empty() && notify_stack.top())
			notify_error(std::string(error));
		else
			log_error(std::string(error));
	}

	breakout_error::notify_scope::notify_scope(bool notify)
	{
		notify_stack.push(notify);
	}

	breakout_error::notify_scope::~notify_scope()
	{
		notify_stack.pop();
	}
}
