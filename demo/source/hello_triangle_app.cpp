#include "hello_triangle_app.h"
#include "log.h"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>



#include <cstring>
#include <vector>
#include <optional>

struct AppContext
{
	GLFWwindow* window{ nullptr };
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice physicalDevice{ VK_NULL_HANDLE };
	VkDevice device;
	VkQueue graphicsQueue;

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif
};

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value();
	}
};





#define ctx m_context


VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}



QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		if (indices.isComplete())
		{
			break;
		}

		i++;
	}

	return indices;
}

bool isDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = findQueueFamilies(device);
	return indices.isComplete();
}



static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	LogError("[vulkan] validation layer: {0}", pCallbackData->pMessage);

	return VK_FALSE;
}





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
	setupDebugMessenger();
	pickPhysicalDevice();
	createLogicalDevice();
}

void HelloTriangleApplication::createInstance()
{
	if (ctx->enableValidationLayers && !checkValidationLayerSupport())
	{
		LogFatal("validation layers requested, but not available!");
		return;
	}


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


	auto extensions = getRequiredExtension();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (ctx->enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(ctx->validationLayers.size());
		createInfo.ppEnabledLayerNames = ctx->validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}
	

	VkResult result = vkCreateInstance(&createInfo, nullptr, &ctx->instance);

	if (result != VK_SUCCESS)
	{
		LogFatal("failed to create vulkan instance");
	}
}

bool HelloTriangleApplication::checkValidationLayerSupport()
{
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : ctx->validationLayers)
	{
		bool layerFound = false;
		for (const auto& layerPropertices : availableLayers)
		{
			if (strcmp(layerName, layerPropertices.layerName) == 0)
			{
				layerFound = true;
			}
		}
		if (!layerFound)
		{
			return false;
		}
	}

	return true;
}

void HelloTriangleApplication::setupDebugMessenger()
{
	if (!ctx->enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = ctx;



	if (CreateDebugUtilsMessengerEXT(ctx->instance, &createInfo, nullptr, &ctx->debugMessenger) != VK_SUCCESS)
	{
		LogFatal("failed to set up debug messenger!");
		return;
	}
}





void HelloTriangleApplication::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(ctx->instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		LogFatal("failed to find GPUs with vulkan support!");
		return;
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(ctx->instance, &deviceCount, devices.data());
	
	for (const auto& device : devices)
	{
		if (isDeviceSuitable(device))
		{
			ctx->physicalDevice = device;
			return;
		}
	}

	LogFatal("failed to find a suitable GPU!");
}

void HelloTriangleApplication::createLogicalDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(ctx->physicalDevice);

	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
	queueCreateInfo.queueCount = 1;
	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = 0;

	if (ctx->enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(ctx->validationLayers.size());
		createInfo.ppEnabledLayerNames = ctx->validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(ctx->physicalDevice, &createInfo, nullptr, &ctx->device) != VK_SUCCESS)
	{
		LogFatal("failed to create logical device!");
		return;
	}

	vkGetDeviceQueue(ctx->device, indices.graphicsFamily.value(), 0, &ctx->graphicsQueue);

}

std::vector<const char*> HelloTriangleApplication::getRequiredExtension()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	if (ctx->enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	return extensions;
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

	vkDestroyDevice(ctx->device, nullptr);

	if (ctx->enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(ctx->instance, ctx->debugMessenger,nullptr);
	}

	vkDestroyInstance(ctx->instance,nullptr);

	if (ctx->window)
	{
		glfwDestroyWindow(ctx->window);
		glfwTerminate();
	}

	
	delete ctx;
}
