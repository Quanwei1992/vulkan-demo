set(this vulkan)
add_library(${this} STATIC IMPORTED GLOBAL)
target_include_directories(${this} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(${this} PROPERTIES 
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/win/vulkan-1.lib
)