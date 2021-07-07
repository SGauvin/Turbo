cmake_minimum_required(VERSION 3.12)

add_compile_definitions(TURBO_ENABLE_CLIENT_LOG)
add_compile_definitions(TURBO_ENABLE_ENGINE_LOG)
add_compile_definitions(TURBO_ENABLE_ASSERT)

project(game)

include_directories(include)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

# ENGINE
add_subdirectory(engine)
target_link_libraries(${CMAKE_PROJECT_NAME} turbo)
