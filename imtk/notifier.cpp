#include "notifier.hpp"

#include <algorithm>

namespace imtk
{
	static imp::functional_event<notification> receive_notification;

	notification::notification(log_level level, std::string message, float timer)
		: level(level), message(std::move(message)), timer(timer)
	{
	}

	void notification::update()
	{
		age += ImGui::GetIO().DeltaTime;
	}
	
	float notification::age_alpha() const
	{
		return std::clamp(1.f - age / timer, 0.f, 1.f);
	}

	bool notification::expired() const
	{
		return age >= timer;
	}

	imp::functional_event<notification>& on_receive_notification()
	{
		return receive_notification;
	}

	void notify(log_level level, std::string message, float timer)
	{
		receive_notification.invoke(notification(level, std::move(message), timer));
	}

	void notify_info(std::string message, float timer)
	{
		notify(log_level::info, std::move(message), timer);
	}

	void notify_success(std::string message, float timer)
	{
		notify(log_level::success, std::move(message), timer);
	}

	void notify_warning(std::string message, float timer)
	{
		notify(log_level::warning, std::move(message), timer);
	}

	void notify_error(std::string message, float timer)
	{
		notify(log_level::error, std::move(message), timer);
	}
}
