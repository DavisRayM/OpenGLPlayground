cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(OpenGLPlayground LANGUAGES C CXX)

find_package(OpenGL REQUIRED)

add_subdirectory(vendor)
add_subdirectory(include)

add_executable(BuildTest)

target_sources(BuildTest
  PRIVATE
    src/main.cc
)
target_link_libraries(BuildTest
  PRIVATE
    OpenGL::GL
    glfw
    glad
    toolkit
)
