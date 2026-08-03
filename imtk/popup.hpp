#pragma once

#include <imgui.h>

#include <string>
#include <optional>

namespace imtk
{
	enum class center_window : char
	{
		never,
		always,
		appearing
	};

	struct popup_config
	{
		center_window center_window = center_window::never;
		bool modal = false;
		ImGuiWindowFlags window_flags = 0;
	};

	class popup
	{
		std::string _name;
		bool _trigger_open = false;
		popup_config _default_config;

		struct draw_impl
		{
		private:
			bool _alive;
			bool _open;

			friend popup;
			draw_impl(const char* name, popup_config config);

		public:
			draw_impl(const draw_impl&) = delete;
			draw_impl(draw_impl&&) noexcept;
			~draw_impl();
			draw_impl& operator=(draw_impl&&) = delete;

			void close();

			operator bool() const;
		};

	public:
		popup(std::string name, popup_config default_config = {});

		void open();
		bool is_opening() const;
		draw_impl draw(std::optional<popup_config> config_override = std::nullopt);
	};
}
