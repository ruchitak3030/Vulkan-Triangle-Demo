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
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	instanceInfo.enabledExtensionCount = glfwExtensionCount;
	instanceInfo.ppEnabledExtensionNames = glfwExtensions;

	//Global Validation layers to be enabled
	instanceInfo.enabledLayerCount = 0;

	//Create instance
	VkResult result = vkCreateInstance(&instanceInfo, nullptr, &instance);

	if (result != VK_SUCCESS)
		throw std::runtime_error("Failed to create an instance");
}

void TriangleApplication::InitializeVulkan()
{
	//Create a connection between your application and Vulkan library.
	CreateInstance();
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
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();
}
