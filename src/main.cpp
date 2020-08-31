#include <iostream>
#include <glfw/glfw3.h>

int main(int argc, char** argv)
{
	int result = glfwInit();
	if (result != GL_TRUE)
	{
		const char* buffer = new char[512];
		glfwGetError(&buffer);
		std::cout << "Failed to initialize GLFW: " << std::endl << buffer << std::endl;
	}
}