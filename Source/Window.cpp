#include "Window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "RenderAPI.h"

namespace mag
{
	const int WIDTH = 800;
	const int HEIGHT = 600;

	Window::Window(const std::string& title) : 
		_window(nullptr), _title(title)
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		_window = glfwCreateWindow(WIDTH, HEIGHT, _title.c_str(), nullptr, nullptr);
		if (!_window)
		{

		}

		RenderAPI::Init(this);
	}

	Window::~Window()
	{
		RenderAPI::Shutdown();

		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	void Window::PollEvents() const
	{
		glfwPollEvents();
	}

	bool Window::ShouldClose()
	{
		return glfwWindowShouldClose(_window);
	}
}
