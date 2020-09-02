#include "PlotWindow.hpp"

#include <iostream>
#include <string>
#include <fstream>

#define frand() ((float)rand() / (float)RAND_MAX)

PlotWindow::PlotWindow(int w, int h, int id, std::string title) :
	window(nullptr), id(id), 
	vertices{
		-0.5f,  0.5f, 0.0f, frand(), frand(), frand(),
		 0.5f,  0.5f, 0.0f, frand(), frand(), frand(),
		 0.5f, -0.5f, 0.0f, frand(), frand(), frand(),
		-0.5f, -0.5f, 0.0f, frand(), frand(), frand()
	}, 
	indices{
		0, 1, 3,
		1, 2, 3
	},
	shader(nullptr), VAO(0), VBO(0), EBO(0)
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

	shader = new Shader("shaders/basic.vert", "shaders/basic.frag");

	// Generate buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// Create VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3) * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 3) * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void PlotWindow::Destroy()
{
	glfwMakeContextCurrent(window);

	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	delete shader;
	shader = nullptr;

	glfwDestroyWindow(window);
}

void PlotWindow::GiveContext()
{
	glfwMakeContextCurrent(window);
}

void PlotWindow::Clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PlotWindow::Display()
{
	shader->Use();

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(window);
}

void PlotWindow::FramebufferSizeCallback(GLFWwindow* window, int w, int h)
{
	glfwMakeContextCurrent(window);
	glViewport(0, 0, w, h);
}
