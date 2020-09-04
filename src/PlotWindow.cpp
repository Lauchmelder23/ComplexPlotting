#include "PlotWindow.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <complex>

#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#define PI 3.14159265358979F

#define frand() ((float)rand() / (float)RAND_MAX)
#define indexof(x, y, w) ((y) * w + x)

typedef std::complex<float> fCmplx;

fCmplx test_function(fCmplx z)
{
	return fCmplx(1.0f, 0.0f) / (z - fCmplx(0.5, 0.0))
		+ fCmplx(1.0f, 0.0f) / (z + fCmplx(0.5, 0.0));
}

PlotWindow::PlotWindow(int w, int h, int id, unsigned int detail, std::string title) :
	window(nullptr), id(id), 
	model(glm::mat4(1.0f)), view(glm::mat4(1.0f)), projection(glm::mat4(1.0f)),
	VAO(0), VBO(0), EBO(0)
{
	IMGUI_CHECKVERSION();

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
	
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init((const char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

	ImGui::StyleColorsDark();

	glViewport(0, 0, w, h);
	glEnable(GL_MULTISAMPLE);

	plotShader = new Shader("shaders/plot.vert", "shaders/plot.frag");
	gridShader = new Shader("shaders/grid.vert", "shaders/grid.frag");

	// Generate meshed plane
	if (detail < 2) detail = 2;
	float diff = 2.0f / (float)(detail - 1);
	for (int z = 0; z < detail; z++)
	{
		for (int x = 0; x < detail; x++)
		{
			fCmplx cmplx = test_function(fCmplx(-1.0f + x * diff, -1.0f + z * diff));
			vertices.push_back(
				{
					-1.0f + x * diff,
					std::abs(cmplx),
					-1.0f + z * diff,
					(std::arg(cmplx) + PI) / 2 / PI
				}
			);
			
			if (x < detail - 1 && z < detail - 1)
			{
				indices.push_back(indexof(x, z, detail));
				indices.push_back(indexof(x + 1, z, detail));
				indices.push_back(indexof(x, z + 1, detail));
				indices.push_back(indexof(x + 1, z, detail));
				indices.push_back(indexof(x + 1, z + 1, detail));
				indices.push_back(indexof(x, z + 1, detail));
			}
		}
	}

	// Generate buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// Create VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), (float*)&vertices[0], GL_STATIC_DRAW);

	// Create EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);

	view = glm::rotate(view, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, -3.0f, -3.0f));
	projection = glm::perspective(45.0f, (float)w / (float)h, 0.1f, 10.0f);
}

void PlotWindow::Destroy()
{
	glfwMakeContextCurrent(window);

	delete plotShader;
	plotShader = nullptr;

	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glfwDestroyWindow(window);
}

void PlotWindow::GiveContext()
{
	glfwMakeContextCurrent(window);
}

void PlotWindow::Clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PlotWindow::Display()
{
	// model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	static float transX = 0.0f, transY = 0.0f, transZ = 0.0f;
	static float pitch = 0.0f, yaw = 0.0f, roll = 0.0f;
	static float scale = 1.0f;
	static bool grid = true;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();

	ImGui::Begin("Controls");
	
	if (ImGui::CollapsingHeader("Plot"))
	{
		ImGui::Checkbox("Grid", &grid);
		ImGui::SliderFloat("Scale", &scale, 0.0f, 10.0f);
		ImGui::Separator();
		ImGui::SliderFloat("X", &transX, -5.0f, 5.0f);
		ImGui::SliderFloat("Y", &transY, -5.0f, 5.0f);
		ImGui::SliderFloat("Z", &transZ, -5.0f, 5.0f);
		ImGui::Separator();
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.f, "%.0f°");
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.f, "%.0f°");
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.f, "%.0f°");
	}

	ImGui::End();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(transX, transY, transZ));
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	model = glm::rotate(model, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, roll, glm::vec3(0.0f, 0.0f, 1.0f));

	glBindVertexArray(VAO);

	if (grid)
	{
		gridShader->Use();
		gridShader->SetUniformMat4("model", &model[0][0]);
		gridShader->SetUniformMat4("view", &view[0][0]);
		gridShader->SetUniformMat4("projection", &projection[0][0]);

		glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	}

	plotShader->Use();

	plotShader->SetUniformMat4("model", &model[0][0]);
	plotShader->SetUniformMat4("view", &view[0][0]);
	plotShader->SetUniformMat4("projection", &projection[0][0]);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	glfwSwapBuffers(window);
}

void PlotWindow::FramebufferSizeCallback(GLFWwindow* window, int w, int h)
{
	glfwMakeContextCurrent(window);
	glViewport(0, 0, w, h);
}
