#pragma once

#include <glad/glad.h>

typedef unsigned int ProgramID;

class Shader
{
public:
	Shader(const char* vertShaderPath, const char* fragShaderPath);
	~Shader();

	void Use();

	void SetUniformMat4(const char* location, const float* value) const;

private:

	static void LoadShaderSource(const char* filepath, char** buffer);

private:
	ProgramID program;
};