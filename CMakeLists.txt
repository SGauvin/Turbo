cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_definitions(TURBO_ENABLE_CLIENT_LOG)
add_compile_definitions(TURBO_ENABLE_ENGINE_LOG)
add_compile_definitions(TURBO_ENABLE_ASSERT)

if (MSVC)
    message(FATAL_ERROR "MSVC not setup")
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")
    if (WIN32)
        message(FATAL_ERROR "MinGW not setup")
    endif()
endif()

project(game)

include_directories(include)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

#Threads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} Threads::Threads)

# ENGINE
add_subdirectory(engine)
target_link_libraries(${CMAKE_PROJECT_NAME} turbo)
