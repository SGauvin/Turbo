cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    message("MSVC not setup")
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")
    if (WIN32)
        message("MinGW not setup")
    endif()
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_library(turbo ${SOURCES})
target_include_directories(turbo PUBLIC include)

#Threads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(libs/glfw)

# GLAD
add_subdirectory(libs/glad)

# FMT
add_subdirectory(libs/fmt)

# GLM
add_subdirectory(libs/glm)

# IMGUI
add_subdirectory(libs/imgui)

target_link_libraries(turbo PUBLIC Threads::Threads fmt glm imgui)
target_link_libraries(turbo PRIVATE glfw glad)
