#include <Main.h>
#include <Window.h>

#include <iostream>

void MagMain()
{
	mag::Window window("Test");
	
	while (!window.ShouldClose())
	{
		window.PollEvents();
	}
}