#pragma once

#include "imtk/util.hpp"
#include "imtk/flags.hpp"

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

	class popup
	{
		std::string _name;
		bool _trigger_open = false;
		center_window _center_window;
		bool _modal;
		window_flags _window_flags;

		struct draw_impl
		{
		private:
			bool _alive;
			bool _open;

			friend popup;
			draw_impl(const char* name, center_window center_window, bool modal, window_flags window_flags);

		public:
			draw_impl(const draw_impl&) = delete;
			draw_impl(draw_impl&&) noexcept;
			~draw_impl();
			draw_impl& operator=(draw_impl&&) = delete;

			void close();

			operator bool() const;
		};

	public:
		popup(std::string name, center_window center_window = center_window::never, bool modal = false, window_flags window_flags = {});

		void open();
		bool is_opening() const;
		draw_impl draw(std::optional<center_window> center_window_override = std::nullopt, std::optional<bool> modal_override = std::nullopt,
			enum_override<window_flags> window_flags_override = {});
	};
}
