#pragma once

#include <glad/glad.h>

#include <fstream>
#include <streambuf>

class ShaderProgram
{
public:
	ShaderProgram() = default;

	~ShaderProgram()
	{
		glDeleteShader(fragmentShader_id);
		glDeleteShader(vertexShader_id);
	}

	std::string VertexShader, FragmentShader;

	int Build(char** error)
	{
		int success;

		// Vertex Shader initialization
		if (VertexShader != "")
		{
			vertexShader_id = glCreateShader(GL_VERTEX_SHADER);
			char* source = new char;
			LoadFile(VertexShader, &source);
			glShaderSource(vertexShader_id, 1, &source, NULL);
			glCompileShader(vertexShader_id);

			glGetShaderiv(vertexShader_id, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertexShader_id, 512, NULL, *error);
				return success;
			}
		}

		// Fragment Shader initialization
		if (FragmentShader != "")
		{
			fragmentShader_id = glCreateShader(GL_FRAGMENT_SHADER);
			char* source = new char;
			LoadFile(FragmentShader, &source);
			glShaderSource(fragmentShader_id, 1, &source, NULL);
			glCompileShader(fragmentShader_id);

			glGetShaderiv(fragmentShader_id, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragmentShader_id, 512, NULL, *error);
				return success;
			}
		}

		// Link shaders
		program_id = glCreateProgram();
		glAttachShader(program_id, vertexShader_id);
		glAttachShader(program_id, fragmentShader_id);
		glLinkProgram(program_id);

		glGetProgramiv(program_id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program_id, 512, NULL, *error);
			return success;
		}

		return 1;
	}

	void Use()
	{
		glUseProgram(program_id);
	}

private:
	void LoadFile(std::string path, char** source)
	{
		std::ifstream file(path);
		std::string shader(
			(std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>()
		);
		size_t s = shader.size();
		*source = (char*)realloc(*source, s + sizeof(char));
		memcpy_s(*source, s, shader.c_str(), s);
		(*source)[s] = '\0';
	}

	unsigned int vertexShader_id, fragmentShader_id, program_id;
};