#include "TriangleApplication.h"



TriangleApplication::TriangleApplication()
{
}


TriangleApplication::~TriangleApplication()
{
}

void TriangleApplication::Run()
{
	InitializeWindow();
	InitializeVulkan();
	MainLoop();
	CleanUp();
}

void TriangleApplication::InitializeWindow()
{
	glfwInit();												//Initialize window

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);			//Since by default glfw is created for OpenGl we need to tell not to create OpenGL context
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);				//No resizing

	window = glfwCreateWindow(WIDTH, HEIGHT, "VULKAN DEMO", nullptr, nullptr);
}

void TriangleApplication::InitializeVulkan()
{
}

void TriangleApplication::MainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();								//Checks for events such as button clicks till the window is closed
	}
}

void TriangleApplication::CleanUp()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}
