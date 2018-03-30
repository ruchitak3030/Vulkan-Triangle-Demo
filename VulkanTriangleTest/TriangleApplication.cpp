#include "TriangleApplication.h"



TriangleApplication::TriangleApplication()
{
}


TriangleApplication::~TriangleApplication()
{
}



void TriangleApplication::Run()
{
	//Initialize GLFW and create a window
	InitializeWindow();

	//Initialize the private objects for the vulkan triangle class
	InitializeVulkan();

	//The main function loop that handles rendering and iterates till the window is closed
	MainLoop();

	//Resource deallocation
	CleanUp();
}


void TriangleApplication::InitializeWindow()
{
	glfwInit();												//Initialize window

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);			//Since by default glfw is created for OpenGl we need to tell not to create OpenGL context
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);				//No resizing

	window = glfwCreateWindow(WIDTH, HEIGHT, "VULKAN DEMO", nullptr, nullptr);
}

void TriangleApplication::CreateInstance()
{
	//Application Information = Provides some useful information to the driver to optimize our application
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.pApplicationName = "TRIANGLE DEMO";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "NO ENGINE";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	//Instance information = tells the Vulkan driver which global extension and validation layers we want to use.
	VkInstanceCreateInfo instanceInfo = {};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &appInfo;

	//Global extension. Since Vulkan is platform agnostic API will need extension to interface with the window system.
	auto extensions = GetRequiredExtensions();
	instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	instanceInfo.ppEnabledExtensionNames = extensions.data();

	//Global Validation layers to be enabled
	if (enableValidationLayers)
	{
		instanceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		instanceInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		instanceInfo.enabledLayerCount = 0;
	}
		

	if (enableValidationLayers && !CheckValidationLayerSupport())
		throw std::runtime_error("Validation layer requested but not available");

	//Create instance
	VkResult result = vkCreateInstance(&instanceInfo, nullptr, &instance);

	if (result != VK_SUCCESS)
		throw std::runtime_error("Failed to create an instance");
}

//Checks if requested layers are available or not
bool TriangleApplication::CheckValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	//Check if all the layers in validatio layer (vulkan SDK validation layers) is found in the available layers.
	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;
		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
			return false;
	}
		


	return true;
}

//Returns the required list of extensions based on the valudation layer is enabled or not
std::vector<const char*> TriangleApplication::GetRequiredExtensions()
{
	//Extension to create an interface between vulkan and window system and use that extension to send the debug messgae.
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	//Sets the callback to receive the debug messages
	if (enableValidationLayers)
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

	return extensions;
}

void TriangleApplication::SetUpDebugCallBack()
{
	if (!enableValidationLayers)
		return;

	//Structure for details of callback
	VkDebugReportCallbackCreateInfoEXT debugInfo;
	debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	debugInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	debugInfo.pfnCallback = debugCallback;

	if (CreateDebugReportCallbackEXT(instance, &debugInfo, nullptr, &callback) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to set up debug callback");
	}
}

void TriangleApplication::SelectPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("Failed to find GPU with Vulkan support");
	}
		
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	//Checks for a suitable device and assigns it to the physicaldevice
	for (const auto& device : devices)
	{
		if (isDeviceSuitable(device))
		{
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE)
		throw std::runtime_error("failed to find suitable GPU");

}

//Evaluates the suitability of the device
bool TriangleApplication::isDeviceSuitable(VkPhysicalDevice device)
{

	//VkPhysicalDeviceProperties deviceProperties;
	//VkPhysicalDeviceFeatures deviceFeatures;

	//vkGetPhysicalDeviceProperties(device, &deviceProperties);
	//vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	////Check whether graphics card supports geometric shaders
	//return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;

	//Checks for devices with suitable queue families supported
	QueueFamilyIndices indices = FindQueueFamilies(device);
	return indices.isComplete();

	
}

//Find QueueFamilies supported by the device that support graphics commands
QueueFamilyIndices TriangleApplication::FindQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}
		if (indices.isComplete())
			break;

		i++;
	}


	return indices;
}

void TriangleApplication::InitializeVulkan()
{
	//Create a connection between your application and Vulkan library.
	CreateInstance();
	SetUpDebugCallBack();

	//Selects a graphics card that supports the features we need.
	SelectPhysicalDevice();
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

	if (enableValidationLayers)
		DestroyDebugReportCallbackEXT(instance, callback, nullptr);

	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();
}

VKAPI_ATTR VkBool32 VKAPI_CALL TriangleApplication::debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char * layerPrefix, const char * msg, void * userData)
{
	std::cerr << "validation layer: " << msg << std::endl;

	return VK_FALSE;
}



