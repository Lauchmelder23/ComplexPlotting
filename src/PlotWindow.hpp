#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include <imgui/imgui.h>

#include "Shader.hpp"

typedef unsigned int BufferObject;

typedef struct
{
	float x, y, z;
	float arg;
} Vertex;

class PlotWindow
{
public:
	PlotWindow(int w, int h, int id, unsigned int detail, std::string title);

	bool ShouldClose() { return glfwWindowShouldClose(window); }
	void Destroy();

	void GiveContext();
	virtual void Clear();
	virtual void Display();

private:
	int id;
	GLFWwindow* window;

	static void FramebufferSizeCallback(GLFWwindow* window, int w, int h);
	BufferObject VAO, VBO, EBO;

	Shader* plotShader, *gridShader;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	glm::mat4 model, view, projection;

	static inline bool isInit = false;
};