#include "hello_triangle_app.h"
#include "log.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

void HelloTriangleApplication::run()
{
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}

void HelloTriangleApplication::initWindow()
{
	if (!glfwInit())
	{
		LogFatal("Failed to initialzie glfw");
		return;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	if (!m_window)
	{
		LogFatal("Failed to create window");
		return;
	}


}

void HelloTriangleApplication::initVulkan()
{
}

void HelloTriangleApplication::mainLoop()
{
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
	}
}

void HelloTriangleApplication::cleanup()
{
	if (m_window)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
}
