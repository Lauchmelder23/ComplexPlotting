#include "PlotWindow.hpp"

#include <iostream>
#include <string>

PlotWindow::PlotWindow(int w, int h, int id, std::string title) :
	window(nullptr), id(id)
{
	window = glfwCreateWindow(w, h, ("Plot " + std::to_string(id) + " | " + title).c_str(), NULL, NULL);
	if (window == nullptr)
	{
		const char* buffer = new const char[512];
		glfwGetError(&buffer);
		std::cerr << "Failed to initialize GLFWwindow: " << std::endl << buffer << std::endl;
		delete[] buffer;
		buffer = nullptr;
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return;
	}

	glViewport(0, 0, w, h);
}

void PlotWindow::Destroy()
{
	glfwDestroyWindow(window);
}

void PlotWindow::GiveContext()
{
	glfwMakeContextCurrent(window);
}

void PlotWindow::Clear()
{
	glClearColor(0.6f, 0.1f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PlotWindow::Display()
{
	glfwSwapBuffers(window);
}

void PlotWindow::FramebufferSizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}
