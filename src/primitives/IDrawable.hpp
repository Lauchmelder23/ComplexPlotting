#pragma once

#include <glad/glad.h>

class IDrawable
{
public:
	virtual void Draw() = 0;

protected:
	IDrawable()
	{
		glCreateVertexArrays(1, &VAO);
		glCreateBuffers(1, &VBO);
	}

	unsigned int VBO, VAO;
};