#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <cstdlib>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "TriangleApplication.h"

int main() 
{
	TriangleApplication application;

	try
	{
		application.Run();
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}