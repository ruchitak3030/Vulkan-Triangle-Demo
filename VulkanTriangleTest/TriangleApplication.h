#pragma once
#define GLFW_INCLUDE_VULKAN					//will automatically include all the vulkan stuff
#include <GLFW\glfw3.h>
#include <stdexcept>
#include <iostream>
#include <functional>
#include <cstdlib>
#include <vector>

const int WIDTH = 800;
const int HEIGHT = 600;

const std::vector<const char*> validationLayers = { "VK_LAYER_LUNARG_standard_validation" };

#ifndef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif // !NDEBUG


//Since the SetUpCallBack() is an extension function, it is not automatically loaded. We need to address it ourselves.
VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}


class TriangleApplication
{
public:
	TriangleApplication();
	~TriangleApplication();
	void Run();

private:

	GLFWwindow* window;

	VkInstance instance;
	VkDebugReportCallbackEXT callback;

	void InitializeWindow();				
	void CreateInstance();	

	bool CheckValidationLayerSupport();
	std::vector<const char*> GetRequiredExtensions();
	void SetUpDebugCallBack();

	void InitializeVulkan();				
	void MainLoop();						
	void CleanUp();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);

};

