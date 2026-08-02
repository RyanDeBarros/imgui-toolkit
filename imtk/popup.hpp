#pragma once

#include <string>

// TODO don't include imgui.h: wrap ImGuiWindowFlags
#include <imgui.h>

namespace imtk
{
	class popup
	{
		std::string _name;
		bool _trigger_open = false;

		struct draw_impl
		{
		private:
			bool _alive;
			bool _open;

			friend popup;
			draw_impl(const char* name, bool modal, ImGuiWindowFlags window_flags);

		public:
			draw_impl(const draw_impl&) = delete;
			draw_impl(draw_impl&&) noexcept;
			~draw_impl();
			draw_impl& operator=(draw_impl&&) = delete;

			void close();

			operator bool() const;
		};

	public:
		popup(std::string name);

		void open();
		bool is_opening() const;
		draw_impl draw(bool modal = false, ImGuiWindowFlags window_flags = 0);
	};
}
