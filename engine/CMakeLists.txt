cmake_minimum_required(VERSION 3.12)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_library(turbo ${SOURCES})

set_property(TARGET turbo PROPERTY CXX_STANDARD 20)
set_property(TARGET turbo PROPERTY CXX_STANDARD_REQUIRED ON)

target_include_directories(turbo PUBLIC include)

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

target_link_libraries(turbo PUBLIC ${TURBO_PUBLIC_LIBS})
target_link_libraries(turbo PRIVATE glfw glad)
