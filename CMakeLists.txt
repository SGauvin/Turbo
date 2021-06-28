cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(turbo)

include_directories(include/turbo)
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

add_custom_target(run)
add_custom_command(
  TARGET   run
  COMMAND  "./${CMAKE_PROJECT_NAME}"
)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(engine/libs/glfw)
target_link_libraries(${CMAKE_PROJECT_NAME} glfw)

# GLAD
add_subdirectory(engine/libs/glad)
target_link_libraries(${CMAKE_PROJECT_NAME} glad)

# FMT
add_subdirectory(engine/libs/fmt)
target_link_libraries(${CMAKE_PROJECT_NAME} fmt)

# GLM
add_subdirectory(engine/libs/glm)
target_link_libraries(${CMAKE_PROJECT_NAME} glm)

# IMGUI
add_subdirectory(engine/libs/imgui)
target_link_libraries(${CMAKE_PROJECT_NAME} imgui)
