#pragma once
#include <cstdint>
#include <vector>

struct AppContext;

class HelloTriangleApplication
{
public:
	void run();
private:
	void initWindow();

	
	void initVulkan();
	void createInstance();
	bool checkValidationLayerSupport();
	void setupDebugMessenger();
	void pickPhysicalDevice();
	void createLogicalDevice();


	void mainLoop();
	void cleanup();

private:
	std::vector<const char*> getRequiredExtension();
	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 600;
	AppContext* m_context{ nullptr };
};