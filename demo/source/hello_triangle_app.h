#pragma once
#include <cstdint>

struct GLFWwindow;

class HelloTriangleApplication
{
public:
	void run();
private:
	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();

	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 600;
	GLFWwindow* m_window{nullptr};
};