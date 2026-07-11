#include "os_window.hpp"

#include "imtk/errors.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace imtk
{
	os_window::os_window(int width, int height, const char* title)
	{
        if (!glfwInit())
            throw error(error_code::init_glfw);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        _w = glfwCreateWindow(width, height, title, nullptr, nullptr);
        glfwMakeContextCurrent(_w);

        if (glewInit() != GLEW_OK)
        {
            glfwDestroyWindow(_w);
            glfwTerminate();
            _w = nullptr;
            throw error(error_code::init_glew);
        }

        glfwSwapInterval(1);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        int monitor_x, monitor_y;
        glfwGetMonitorPos(monitor, &monitor_x, &monitor_y);
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowPos(_w, monitor_x + mode->width / 2, monitor_y + mode->height / 2);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        float monitor_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(monitor);
        ImGui::StyleColorsDark();
        ImGui::GetStyle().ScaleAllSizes(monitor_scale);

        ImGui_ImplGlfw_InitForOpenGL(_w, true);
        ImGui_ImplOpenGL3_Init("#version 150");

        ImGuiIO& io = ImGui::GetIO();
        io.FontGlobalScale = monitor_scale;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	}

	os_window::~os_window()
	{
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        if (_w)
        {
            glfwDestroyWindow(_w);
            glfwTerminate();
        }
	}

	const GLFWwindow* os_window::get() const
	{
		return _w;
	}

	GLFWwindow* os_window::get()
	{
		return _w;
	}

	void os_window::set_size(int width, int height) const
	{
		int x, y;
		int w, h;
		glfwGetWindowPos(_w, &x, &y);
		glfwGetWindowSize(_w, &w, &h);

		const float scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
		const int nw = static_cast<int>(width * scale);
		const int nh = static_cast<int>(height * scale);
		glfwSetWindowSize(_w, nw, nh);

		const int nx = x + (w - nw) / 2;
		const int ny = y + (h - nh) / 2;
		glfwSetWindowPos(_w, nx, ny);
	}

	void os_window::set_maximized(bool maximized) const
	{
		if (maximized)
			glfwMaximizeWindow(_w);
		else
			glfwRestoreWindow(_w);
	}

	void os_window::set_fullscreen(bool fullscreen) const
	{
		if (fullscreen == _state.fullscreen)
			return;

		_state.fullscreen = fullscreen;
		if (fullscreen)
		{
			glfwGetWindowPos(_w, &_state.x, &_state.y);
			glfwGetWindowSize(_w, &_state.w, &_state.h);

			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(_w, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else
		{
			glfwSetWindowMonitor(_w, nullptr, _state.x, _state.y, _state.w, _state.h, 0);
		}
	}

	bool os_window::is_fullscreen() const
	{
		return _state.fullscreen;
	}
}
