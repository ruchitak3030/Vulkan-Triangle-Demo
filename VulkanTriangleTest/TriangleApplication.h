#pragma once
#define GLFW_INCLUDE_VULKAN					//will automatically include all the vulkan stuff
#include <GLFW\glfw3.h>
#include <stdexcept>
#include <iostream>
#include <functional>
#include <cstdlib>

const int WIDTH = 800;
const int HEIGHT = 600;

class TriangleApplication
{
public:
	TriangleApplication();
	~TriangleApplication();
	void Run();

private:

	GLFWwindow* window;

	void InitializeWindow();				//Initialize GLFW and create a window
	void InitializeVulkan();				//Initialize the private objects for the vulkan triangle class
	void MainLoop();						//The main function loop that handles rendering and iterates till the window is closed
	void CleanUp();							//Resource deallocation

};

