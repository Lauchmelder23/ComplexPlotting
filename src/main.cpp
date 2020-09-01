#include <iostream>
#include <vector>
#include <algorithm>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "PlotManager.hpp"

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

	while (PlotManager::isOpen)
	{
		glfwPollEvents();
		PlotManager::Loop();
	}
}