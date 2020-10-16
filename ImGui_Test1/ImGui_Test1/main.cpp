﻿#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"ImGui/imgui.h"
#include"ImGui/impl/imgui_impl_glfw.h"
#include"ImGui/impl/imgui_impl_opengl3.h"

#include"SnowyArkGui.h"

#include<GLShader.h>

#include<iostream>

void ProcessInput(GLFWwindow* window);
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

//setting
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


bool showSnowyArkGui = true;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

int main()
{
	//glfw:initialize
	glfwInit();
	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _APPLE_
	glfwInitHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // _APPLE_

	//glfw:create window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	//glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	GLShader shader("Shader/vertexShader.vert", "Shader/fragmentShader.frag");

	//set ImGui
	//--------------------------------
	//set ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	//setup ImGui style
	ImGui::StyleColorsDark();		//DarkStyle
	//ImGui::StyleColorsLight();		//LightStyle
	//ImGui::StyleColorsClassic();	//ClassicStyle

	//setup platform/renderer bindings
	const char* glsl_version = "#version 330";	//OpenGL3.3

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);



	//set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
	};

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//input
		ProcessInput(window);

		//start the ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//Create ImGui
		if(showSnowyArkGui)
			ShowSnowyArkImGuiWindow(&showSnowyArkGui);

		//render
		glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//render the triangle
		/*shader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);*/

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(width, height, NULL, NULL);
}
