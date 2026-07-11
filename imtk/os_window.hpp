#pragma once

#include <memory>

struct GLFWwindow;

namespace imtk
{
	class os_window
	{
		GLFWwindow* _w = nullptr;

		struct state
		{
			int x = 0, y = 0;
			int w = 1, h = 1;
			bool fullscreen = false;
		};

		mutable state _state;

	public:
		os_window(int width, int height, const char* title);
		os_window(const os_window&) = delete;
		os_window(os_window&&) = delete;
		~os_window();
		
		const GLFWwindow* get() const;
		GLFWwindow* get();

		void set_size(int width, int height) const;
		void set_maximized(bool maximized) const;
		void set_fullscreen(bool fullscreen) const;
		bool is_fullscreen() const;
	};
}
