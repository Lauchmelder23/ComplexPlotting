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
	vert(0), frag(0), shader(0), VAO(0), VBO(0), EBO(0)
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

	int success;
	char errorBuf[512];

	// Load and compile Vertex Shader
	vert = glCreateShader(GL_VERTEX_SHADER);
	char* shaderSource = nullptr;
	LoadShaderSourceCode("shaders/basic.vert", &shaderSource);
	if (shaderSource == nullptr)
		shaderSource = "";

	glShaderSource(vert, 1, &shaderSource, NULL);
	glCompileShader(vert);
	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vert, 512, NULL, errorBuf);
		std::cerr << "Vertex shader compilation error" << std::endl << errorBuf << std::endl;
		glfwSetWindowShouldClose(window, 1);
		return;
	}

	// load and compile Fragment Shader
	frag = glCreateShader(GL_FRAGMENT_SHADER);
	shaderSource = nullptr;
	LoadShaderSourceCode("shaders/basic.frag", &shaderSource);
	if (shaderSource == nullptr)
		shaderSource = "";

	glShaderSource(frag, 1, &shaderSource, NULL);
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(frag, 512, NULL, errorBuf);
		std::cerr << "Fragment shader compilation error" << std::endl << errorBuf << std::endl;
		glfwSetWindowShouldClose(window, 1);
		return;
	}

	// Link shaders into shader program
	shader = glCreateProgram();
	glAttachShader(shader, vert);
	glAttachShader(shader, frag);
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader, 512, NULL, errorBuf);
		std::cerr << "Shader program linking error" << std::endl << errorBuf << std::endl;
		glfwSetWindowShouldClose(window, 1);
		return;
	}

	glDeleteShader(frag);
	glDeleteShader(vert);

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

	glDeleteProgram(shader);

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
	glUseProgram(shader);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(window);
}

void PlotWindow::FramebufferSizeCallback(GLFWwindow* window, int w, int h)
{
	glfwMakeContextCurrent(window);
	glViewport(0, 0, w, h);
}

void PlotWindow::LoadShaderSourceCode(const char* path, char** buffer)
{
	std::ifstream file(path);
	if (!file.good())
	{
		std::cerr << "File not found: " << path << std::endl;
		*buffer = nullptr;
		return;
	}

	std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	size_t len = code.size();
	*buffer = (char*)malloc(len + sizeof(char));
	if (*buffer == nullptr)
		return;
	
	memcpy_s(*buffer, len, code.c_str(), len);
	(*buffer)[len] = '\0';
}
