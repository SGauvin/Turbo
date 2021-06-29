cmake_minimum_required(VERSION 3.10)
project(imgui)
add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)

file(GLOB SOURCES "imgui/*.cpp")
add_library(imgui ${SOURCES} imgui/backends/imgui_impl_opengl3.cpp imgui/backends/imgui_impl_glfw.cpp)
target_include_directories(imgui PUBLIC . imgui) 
target_include_directories(imgui PRIVATE ../glad/include) 
