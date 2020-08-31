#include <iostream>
#include <vector>
#include <algorithm>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "PlotWindow.hpp"

int main(int argc, char** argv)
{
	int result = glfwInit();
	if (result != GL_TRUE)
	{
		const char* buffer = new char[512];
		glfwGetError(&buffer);
		std::cout << "Failed to initialize GLFW: " << std::endl << buffer << std::endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	std::vector<PlotWindow> windows;
	windows.emplace_back(500, 500, 1);
	windows.emplace_back(500, 500, 2);
	windows.emplace_back(500, 500, 3);

	while (!windows.empty())
	{
		glfwPollEvents();

		for (std::vector<PlotWindow>::iterator it = windows.begin(); it != windows.end();)
		{
			if (it->ShouldClose())
			{
				it->Destroy();
				it = windows.erase(it);
				continue;
			}

			it->GiveContext();
			it->Display();
			it++;
		}
	}
}