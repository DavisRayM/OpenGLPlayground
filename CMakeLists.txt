cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

project(OpenGLPlayground LANGUAGES C CXX)

find_package(OpenGL REQUIRED)

add_subdirectory(vendor)
add_subdirectory(include)
add_subdirectory(src)
