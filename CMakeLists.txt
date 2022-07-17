cmake_minimum_required(VERSION 3.12)

add_compile_definitions(TURBO_ENABLE_CLIENT_LOG)
add_compile_definitions(TURBO_ENABLE_ENGINE_LOG)
add_compile_definitions(TURBO_ENABLE_ASSERT)

project(game)

include_directories(include)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}")
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)

# ENGINE
add_subdirectory(engine)
target_link_libraries(${CMAKE_PROJECT_NAME} turbo)
