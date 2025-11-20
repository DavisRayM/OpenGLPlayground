add_executable(HelloTriangle)

target_sources(HelloTriangle
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
)

target_link_libraries(HelloTriangle
  PUBLIC
    OpenGL::GL
    glfw
    glad
)
