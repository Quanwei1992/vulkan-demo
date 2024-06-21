
add_subdirectory(3rdparty/glfw)
add_subdirectory(3rdparty/spdlog)
add_subdirectory(3rdparty/glm)
add_subdirectory(3rdparty/vulkan)

ucm_add_dirs(source TO SOURCES RECURSIVE)

add_executable(VulkanDemo ${SOURCES})
target_link_libraries(VulkanDemo PRIVATE glfw spdlog glm vulkan)