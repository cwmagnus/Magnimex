#pragma once

#include <string>

struct GLFWwindow;

namespace mag
{

	class Window final
	{
	public:
		Window(const std::string& title);
		~Window();

		void PollEvents() const;
		bool ShouldClose();

		inline GLFWwindow* GetGLFWwindow() const
		{
			return _window;
		}

		inline std::string GetTitle() const
		{
			return _title;
		}

	private:
		GLFWwindow* _window;
		std::string _title;
	};
}
