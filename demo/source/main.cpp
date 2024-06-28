#include "hello_triangle_app.h"

#include "log.h"

#include <iostream>
#include <stdexcept>
#include <cstdlib>


struct AutoInitiazeCleanupRumtime
{
	AutoInitiazeCleanupRumtime()
	{
		LogSystem::initialize();
	}
	~AutoInitiazeCleanupRumtime()
	{
		LogSystem::shutdown();
	}
};

int main()
{
	AutoInitiazeCleanupRumtime _;

	HelloTriangleApplication app;
	app.run();
	return 0;
}