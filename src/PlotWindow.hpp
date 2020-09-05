#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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
	BufferObject VAO, VBO, EBO;

	Shader* plotShader, *gridShader;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	static inline glm::mat4 model = glm::mat4(1.0f);
	static inline glm::quat modelRotation = glm::quat();
	static inline glm::mat4 modelScale = glm::mat4(1.0f);
	static inline glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection;

	static inline bool isInit = false;
	static inline bool isLeftButtonDown = false;
	static inline bool isRightButtonDown = false;

	static inline float scaling = 1.0f;

	static inline glm::vec2 prevCursorPos = glm::vec2(1.0f, 1.0f);

	static inline int width = 0, height = 0;

private:
	static void FramebufferSizeCallback(GLFWwindow* window, int w, int h);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
};