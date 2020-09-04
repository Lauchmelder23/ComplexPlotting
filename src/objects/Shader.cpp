#include "Shader.hpp"

#include <iostream>
#include <fstream>

Shader::Shader(const char* vertShaderPath, const char* fragShaderPath) :
	program(0)
{
	int success;
	char errorBuffer[512];

	// Create Vertex shader
	unsigned int vert;
	vert = glCreateShader(GL_VERTEX_SHADER);
	char* sourceBuffer;
	LoadShaderSource(vertShaderPath, &sourceBuffer);
	if (sourceBuffer == nullptr)
		sourceBuffer = "";

	glShaderSource(vert, 1, &sourceBuffer, NULL);
	glCompileShader(vert);
	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vert, 512, NULL, errorBuffer);
		std::cerr << vertShaderPath << std::endl << errorBuffer << std::endl;
		return;
	}

	// Create Fragment shader
	unsigned int frag;
	frag = glCreateShader(GL_FRAGMENT_SHADER);
	LoadShaderSource(fragShaderPath, &sourceBuffer);
	if (sourceBuffer == nullptr)
		sourceBuffer = "";

	glShaderSource(frag, 1, &sourceBuffer, NULL);
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(frag, 512, NULL, errorBuffer);
		std::cerr << fragShaderPath << std::endl << errorBuffer << std::endl;
		return;
	}

	// Link shaders
	program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, errorBuffer);
		std::cerr << errorBuffer << std::endl;
		return;
	}

	glDeleteShader(frag);
	glDeleteShader(vert);
}

Shader::~Shader()
{
	glDeleteProgram(program);
}

void Shader::Use()
{
	glUseProgram(program);
}

void Shader::SetUniformMat4(const char* location, const float* value) const
{
	glUniformMatrix4fv(glGetUniformLocation(program, location), 1, GL_FALSE, value);
}

void Shader::LoadShaderSource(const char* filepath, char** buffer)
{
	std::ifstream file(filepath);
	if (!file.good())
	{
		std::cerr << "Failed to open file: " << filepath << std::endl;
		*buffer = nullptr;
		return;
	}

	std::string source(
		(std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>()
	);
	size_t len = source.size();

	*buffer = (char*)malloc(len + sizeof(char));
	if (*buffer == nullptr)
	{
		std::cerr << "Failed to allocate memory for shader source code" << std::endl;
		return;
	}

	memcpy_s(*buffer, len, source.c_str(), len);
	(*buffer)[len] = '\0';
}
