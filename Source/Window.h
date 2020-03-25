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

		const char** GetRequiredInstanceExtensions(uint32_t& extensionsCount) const;
		void PollEvents() const;
		bool ShouldClose();

		inline std::string GetTitle() const
		{
			return _title;
		}

	private:
		GLFWwindow* _window;
		std::string _title;
	};
}
