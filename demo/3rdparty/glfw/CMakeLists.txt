set(this glfw)


ucm_add_files(
    "include/GLFW/glfw3.h"
    "include/GLFW/glfw3native.h"
    "src/context.c"
    "src/init.c"
    "src/input.c"
    "src/monitor.c"
    "src/vulkan.c"
    "src/window.c"
    # Windows Only
    "src/win32_platform.h"
    "src/win32_joystick.h"
    "src/osmesa_context.h" 
    "src/win32_init.c" 
    "src/win32_joystick.c" 
    "src/win32_monitor.c" 
    "src/win32_time.c" 
    "src/win32_thread.c" 
    "src/win32_window.c"  
    "src/osmesa_context.c"
TO sources)

if(WIN32)
    ucm_add_files(
        "src/win32_platform.h"
        "src/win32_joystick.h"
        "src/osmesa_context.h" 
        "src/win32_init.c" 
        "src/win32_joystick.c" 
        "src/win32_monitor.c" 
        "src/win32_time.c" 
        "src/win32_thread.c" 
        "src/win32_window.c"  
        "src/wgl_context.c"
        "src/egl_context.c"
        "src/osmesa_context.c"
    TO sources)
endif()


add_library(${this} STATIC ${sources})
target_include_directories(${this} PUBLIC include)
target_compile_definitions(${this} PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_definitions(${this} PUBLIC _GLFW_WIN32)
set_target_properties(${this} PROPERTIES FOLDER 3rdparty)    