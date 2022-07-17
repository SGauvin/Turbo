cmake_minimum_required(VERSION 3.12)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_library(turbo ${SOURCES})

set_property(TARGET turbo PROPERTY CXX_STANDARD 20)
set_property(TARGET turbo PROPERTY CXX_STANDARD_REQUIRED ON)

target_include_directories(turbo PUBLIC include)

target_compile_options(turbo PRIVATE -Wall -Wextra -Wpedantic)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}")
set_target_properties(turbo PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

if (WIN32 AND NOT MSVC)
    # IMM32
    SET(CMAKE_FIND_LIBRARY_PREFIXES "")
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")

    find_library(IMM32 imm32)
    if(NOT IMM32)
        message(FATAL_ERROR "imm32 not found")
    endif()
    set(TURBO_PUBLIC_LIBS ${TURBO_PUBLIC_LIBS} ${IMM32})
else()
    #Threads
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    set(TURBO_PUBLIC_LIBS ${TURBO_PUBLIC_LIBS} Threads::Threads)
endif()

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
target_compile_definitions(turbo PRIVATE GLFW_INCLUDE_NONE)
add_subdirectory(libs/glfw)

# GLAD
add_subdirectory(libs/glad)

# FMT
add_subdirectory(libs/fmt)
set(TURBO_PUBLIC_LIBS ${TURBO_PUBLIC_LIBS} fmt)

# GLM
add_subdirectory(libs/glm)
set(TURBO_PUBLIC_LIBS ${TURBO_PUBLIC_LIBS} glm)

# IMGUI
add_subdirectory(libs/imgui)
set(TURBO_PUBLIC_LIBS ${TURBO_PUBLIC_LIBS} imgui)

# ENTT
add_subdirectory(libs/entt)
set(TURBO_PUBLIC_LIBS ${TURBO_PUBLIC_LIBS} EnTT)

# STB
target_include_directories(turbo PRIVATE libs/stb)

# NLOHMANN JSON
add_compile_definitions(JSON_NOEXCEPTION)
set (JSON_BuildTests OFF CACHE INTERNAL "" FORCE)
add_subdirectory(libs/json)
set(TURBO_PUBLIC_LIBS ${TURBO_PUBLIC_LIBS} nlohmann_json::nlohmann_json)

# TINY GLTF
add_compile_definitions(TINYGLTF_NOEXCEPTION)
set(TINYGLTF_HEADER_ONLY OFF CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE INTERNAL "" FORCE)
set(TINYGLTF_BUILD_GL_EXAMPLES OFF CACHE INTERNAL "" FORCE)
set(TINYGLTF_BUILD_VALIDATOR_EXAMPLE OFF CACHE INTERNAL "" FORCE)
set(TINYGLTF_BUILD_BUILDER_EXAMPLE OFF CACHE INTERNAL "" FORCE)
add_subdirectory(libs/tinygltf)
set(TURBO_PUBLIC_LIBS ${TURBO_PUBLIC_LIBS} tinygltf)

target_link_libraries(turbo PUBLIC ${TURBO_PUBLIC_LIBS})
target_link_libraries(turbo PRIVATE glfw glad)
