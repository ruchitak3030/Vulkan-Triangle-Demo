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
const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

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

struct QueueFamilyIndices
{
	int graphicsFamily = -1;
	int presentFamily = -1;
	bool isComplete()
	{
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};


class TriangleApplication
{
public:
	TriangleApplication();
	~TriangleApplication();
	void Run();

private:

	//GLFW stuff
	GLFWwindow* window;

	//Instance stuff
	VkInstance instance;
	VkDebugReportCallbackEXT callback;

	//Window Surface creation stuff
	VkSurfaceKHR surface;

	//Physical Device Stuff
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	//Logical Device stuff
	VkDevice device;

	//Queue stuff
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	//Swap Chain stuff
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	//Image View stuff
	std::vector<VkImageView> swapChainImageViews;

	//GLFW related functions
	void InitializeWindow();				
	
	//Instance related functions
	void CreateInstance();	
	bool CheckValidationLayerSupport();
	std::vector<const char*> GetRequiredExtensions();
	void SetUpDebugCallBack();

	//Window Surface creation related function
	void CreateSurface();

	//Physical Device related functions
	void SelectPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);

	//Queue Families stuff
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	//Logical Device related functions
	void CreateLogicalDevice();

	//Swap chain creation related functions
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes); 
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void CreateSwapChain();

	//Image View related functions
	void CreateImageView();

	//Graphics Pipeline
	void CreateGraphicsPipeline();
	VkShaderModule CreateShaderModule(std::vector<char>& code);

	void InitializeVulkan();				
	void MainLoop();						
	void CleanUp();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);

	//Function to load binary data from files
	static std::vector<char> readFile(const std::string& filename);
};

