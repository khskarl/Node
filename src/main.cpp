#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <algorithm>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"

#include "opengl_utils.h"

#include "application.h"

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}


ImVec4 gClearColor = ImColor(23, 20, 20);

int main(int, char**)
{
	// Setup window
	glfwSetErrorCallback(error_callback);
	
	if (!glfwInit())
		return 1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Node", NULL, NULL);
	glfwMakeContextCurrent(window);
	gl3wInit();

	LoadAllShaders();	

	// Setup ImGui binding
	ImGui_ImplGlfwGL3_Init(window, true);

	Application app;
	app.Init();

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		// ShowApplication();
		app.Show();
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Rendering
		int displayW, displayH;
		glfwGetFramebufferSize(window, &displayW, &displayH);
		glViewport(0, 0, displayW, displayH);
		glClearColor(gClearColor.x, 
					 gClearColor.y, 
					 gClearColor.z, 
					 gClearColor.w);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui::Render();
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();

	return 0;
}
