
add_subdirectory(3rdparty/glfw)
add_subdirectory(3rdparty/spdlog)

ucm_add_dirs(source TO SOURCES RECURSIVE)

add_executable(VulkanDemo ${SOURCES})
target_link_libraries(VulkanDemo PRIVATE glfw spdlog)