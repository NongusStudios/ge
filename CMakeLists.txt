cmake_minimum_required(VERSION 3.27)
project(ge)

set(CMAKE_CXX_STANDARD 20)

set(PATH_TO_VULKANSDK C:/VulkanSDK/1.3.275.0)
set(VULKANSDK_INC ${PATH_TO_VULKANSDK}/Include)
set(VULKANSDK_LIB ${PATH_TO_VULKANSDK}/Lib/vulkan-1.lib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
        external/g_app/include
        external/g_app/external/glfw/include
        external/g_app/external/vma/include
        external/g_app/external/spdlog/include
        external/g_app/external/imgui
        external/g_app/external/stb_image
        external/g_app/external/miniaudio
        include
)

add_subdirectory(external/g_app)

file(GLOB SRC include/*.hpp src/*.cpp)
set(CMAKE_CONFIGURE_DEPENDS ${SRC})

add_library(ge
        ${SRC}
)

add_executable(out src/main.cpp
        include/ge.hpp)

target_link_libraries(out ge g_app glfw spdlog::spdlog vulkan imgui)

file(GLOB GLSL_FILES
        src/shaders/*.vert
        src/shaders/*.frag
        src/shaders/*.comp
)

foreach(GLSL_FILE ${GLSL_FILES})
    get_filename_component(FILE_NAME ${GLSL_FILE} NAME)
    get_filename_component(FILE_PATH ${GLSL_FILE} DIRECTORY)
    set(SPIRV "${FILE_PATH}/${FILE_NAME}.spv")
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND glslc -o ${SPIRV} ${GLSL_FILE}
            DEPENDS ${GLSL_FILE}
    )
    list(APPEND SPIRV_FILES ${SPIRV})
endforeach(GLSL_FILE)

add_custom_target(shaders ALL
        DEPENDS ${SPIRV_FILES})
