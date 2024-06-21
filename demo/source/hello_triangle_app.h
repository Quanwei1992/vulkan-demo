#pragma once
#include <cstdint>

struct AppContext;

class HelloTriangleApplication
{
public:
	void run();
private:
	void initWindow();

	
	void initVulkan();
	void createInstance();

	void mainLoop();
	void cleanup();

	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 600;
	AppContext* m_context{ nullptr };
};