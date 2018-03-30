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
	VkInstance instance;

	void InitializeWindow();				
	void CreateInstance();		
	void InitializeVulkan();				
	void MainLoop();						
	void CleanUp();							

};

