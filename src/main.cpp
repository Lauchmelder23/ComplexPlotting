#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

#include "ShaderProgram.hpp"
#include "Triangle.hpp"
#include "Quad.hpp"

void LogGLFWerror()
{
	const char* error = new const char;
	glfwGetError(&error);
	std::cerr << error << std::endl;
}

void FramebufferSizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

int main(int argc, char** argv)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(800, 800, "Complex Plot", NULL, NULL);
	if (window == nullptr)
	{
		LogGLFWerror();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "GLAD failed to initialize" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 800);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	// Create Shader Program
	ShaderProgram program;
	program.VertexShader = "shaders/triangle.vert";
	program.FragmentShader = "shaders/triangle.frag";

	char* error = new char;
	if (!program.Build(&error))
	{
		std::cout << error << std::endl;
		return -1;
	}

	// Make triangle
	Triangle t(
		{ -0.5f, -0.5f, 0.f },
		{ 0.5f, -0.5f, 0.f },
		{ 0.f, 0.5, 0.f }
	);

	Quad q(
		{ -0.5f, 0.5f, 0.0f },
		{ 0.5f, 0.5f, 0.0f },
		{ 0.5f, -0.5f, 0.0f },
		{ -0.5f, -0.5f, 0.0f }
	);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		float time = glfwGetTime();
		int timeUniformLocation = glGetUniformLocation(program.program_id, "t");

		glClearColor(0.4f, 0.1f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		program.Use();
		glUniform1f(timeUniformLocation, time);
		q.Draw();

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}