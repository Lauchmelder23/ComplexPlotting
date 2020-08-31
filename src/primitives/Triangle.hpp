#include <memory>

#include "IDrawable.hpp"
#include "util.hpp"

class Triangle : 
	public IDrawable
{
public:
	Triangle(Vec3 v1, Vec3 v2, Vec3 v3)
	{
		memcpy_s(vertices + 0, 9 * sizeof(float), &v1, 3 * sizeof(float));
		memcpy_s(vertices + 3, 9 * sizeof(float), &v2, 3 * sizeof(float));
		memcpy_s(vertices + 6, 9 * sizeof(float), &v3, 3 * sizeof(float));

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Draw() override 
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}

private:
	float vertices[9];
};