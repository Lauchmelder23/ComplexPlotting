#pragma once

#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Shader.hpp"

typedef unsigned int BufferObject;

class PlotWindow
{
public:
	PlotWindow(int w, int h, int id, std::string title);

	bool ShouldClose() { return glfwWindowShouldClose(window); }
	void Destroy();

	void GiveContext();
	virtual void Clear();
	virtual void Display();

private:
	int id;
	GLFWwindow* window;

	static void FramebufferSizeCallback(GLFWwindow* window, int w, int h);

	Shader* shader;
	BufferObject VAO, VBO, EBO;

	float			vertices[4 * 3 * 4];
	unsigned int	indices[2 * 3];
};