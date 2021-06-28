cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(ENGINE_NAME "turbo")

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(${ENGINE_NAME} ${SOURCES})
target_include_directories(${ENGINE_NAME} PUBLIC include)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(libs/glfw)
target_link_libraries(${ENGINE_NAME} glfw)

# GLAD
add_subdirectory(libs/glad)
target_link_libraries(${ENGINE_NAME} glad)

# FMT
add_subdirectory(libs/fmt)
target_link_libraries(${ENGINE_NAME} fmt)

# GLM
add_subdirectory(libs/glm)
target_link_libraries(${ENGINE_NAME} glm)

# IMGUI
add_subdirectory(libs/imgui)
target_link_libraries(${ENGINE_NAME} imgui)
