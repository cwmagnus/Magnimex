#include <Window.h>

#include <iostream>

int main()
{
	mag::Window window("Test");
	
	while (!window.ShouldClose())
	{
		window.PollEvents();
	}

	return 0;
}