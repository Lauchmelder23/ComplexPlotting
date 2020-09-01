#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#pragma once

class PlotWindow
{
public:
	PlotWindow(int w, int h, int id);

	bool ShouldClose() { return glfwWindowShouldClose(window); }
	void Destroy();

	void GiveContext();
	virtual void Clear();
	virtual void Display();

private:
	int id;
	GLFWwindow* window;

	static void FramebufferSizeCallback(GLFWwindow* window, int w, int h);
};