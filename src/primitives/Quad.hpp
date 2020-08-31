#pragma once
#include <memory>

#include "IDrawable.hpp"
#include "util.hpp"

class Quad :
	public IDrawable
{
public:
	Quad(Vec3 topLeft, Vec3 topRight, Vec3 botRight, Vec3 botLeft)
	{
		memcpy_s(vertices + 0, sizeof(vertices), &topLeft, sizeof(topLeft));
		memcpy_s(vertices + 3, sizeof(vertices), &topRight, sizeof(topRight));
		memcpy_s(vertices + 6, sizeof(vertices), &botRight, sizeof(botRight));
		memcpy_s(vertices + 9, sizeof(vertices), &botLeft, sizeof(botLeft));

		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Draw() override
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
	}

private:
	unsigned int EBO;
	float vertices[3 * 4];
	inline static unsigned int indices[6] = {
		0, 1, 3,
		1, 2, 3
	};
};