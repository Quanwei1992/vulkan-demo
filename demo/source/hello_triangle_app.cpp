#include "hello_triangle_app.h"
#include "log.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


struct AppContext
{
	GLFWwindow* window{ nullptr };
	VkInstance instance;
};

#define ctx m_context

void HelloTriangleApplication::run()
{
	m_context = new AppContext();


	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}

void HelloTriangleApplication::initWindow()
{
	if (!glfwInit())
	{
		LogFatal("failed to initialzie glfw");
		return;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	ctx->window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	if (!ctx->window)
	{
		LogFatal("failed to create window");
		return;
	}

	
}

void HelloTriangleApplication::initVulkan()
{
	createInstance();
}

void HelloTriangleApplication::createInstance()
{
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	createInfo.enabledLayerCount = 0;

	VkResult result = vkCreateInstance(&createInfo, nullptr, &ctx->instance);

	if (result != VK_SUCCESS)
	{
		LogFatal("failed to create vulkan instance");
	}

}

void HelloTriangleApplication::mainLoop()
{
	while (!glfwWindowShouldClose(ctx->window))
	{
		glfwPollEvents();
	}
}

void HelloTriangleApplication::cleanup()
{
	vkDestroyInstance(ctx->instance,nullptr);

	if (ctx->window)
	{
		glfwDestroyWindow(ctx->window);
		glfwTerminate();
	}

	
	delete ctx;
}
