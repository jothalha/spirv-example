cmake_minimum_required(VERSION 3.1)
project(OpenGLTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(spirv_helper)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(color)
add_subdirectory(uniform)
add_subdirectory(ubo)
