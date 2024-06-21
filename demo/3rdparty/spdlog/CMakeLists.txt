set(this spdlog) 

ucm_add_dirs(include src TO sources RECURSIVE)
add_library(${this} STATIC ${sources})

target_include_directories(${this} PUBLIC include )
target_include_directories(${this} PRIVATE src )
set_target_properties(${this} PROPERTIES FOLDER 3rdparty)    
target_compile_definitions(${this} PUBLIC SPDLOG_COMPILED_LIB _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)


if(MSVC)
target_compile_options(${this} PRIVATE /wd4996)
endif()