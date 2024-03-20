cmake_minimum_required(VERSION 3.27)
project(ge)

set(CMAKE_CXX_STANDARD 20)

set(PATH_TO_VULKANSDK C:/VulkanSDK/1.3.275.0)
set(VULKANSDK_INC ${PATH_TO_VULKANSDK}/Include)
set(VULKANSDK_LIB ${PATH_TO_VULKANSDK}/Lib/vulkan-1.lib)

include_directories(
        external/g_app/include
        external/g_app/external/glfw/include
        external/g_app/external/vma/include
        external/g_app/external/spdlog/include
        external/g_app/external/imgui
        external/g_app/external/stb_image
        external/g_app/external/miniaudio
        include
        ${VULKANSDK_INC}
)

add_subdirectory(external/g_app)

add_library(ge
        include/ge.hpp
        include/ent.hpp
        src/ent.cpp
        src/ge.cpp
)

add_executable(out src/main.cpp
        include/ge.hpp)

target_link_libraries(out ge g_app glfw spdlog::spdlog vulkan imgui)
