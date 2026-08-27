#pragma once

#include "imtk/logger.hpp"

#include <imp/event.hpp>

namespace imtk
{
	struct notification
	{
		log_level level;
		std::string message;
		float timer;
		float age = 0.f;

		notification(log_level level, std::string message, float timer = 3.f);

		void update();
		float age_alpha() const;
		bool expired() const;
	};

	extern imp::event<notification>& on_receive_notification();

	extern void notify(log_level level, std::string message, float timer = 3.f);
	extern void notify_info(std::string message, float timer = 3.f);
	extern void notify_success(std::string message, float timer = 3.f);
	extern void notify_warning(std::string message, float timer = 3.f);
	extern void notify_error(std::string message, float timer = 3.f);
}
