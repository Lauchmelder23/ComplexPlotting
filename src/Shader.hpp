#pragma once

#include <glad/glad.h>

typedef unsigned int ProgramID;

class Shader
{
public:
	Shader(const char* vertShaderPath, const char* fragShaderPath);
	~Shader();

	void Use();

private:

	static void LoadShaderSource(const char* filepath, char** buffer);

private:
	ProgramID program;
};