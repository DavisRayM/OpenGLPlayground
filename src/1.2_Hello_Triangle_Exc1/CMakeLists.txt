add_executable(HelloTriangleE1)

target_sources(HelloTriangleE1
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
)

target_link_libraries(HelloTriangleE1
  PRIVATE
    OpenGL::GL
    glfw
    glad
)
