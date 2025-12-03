add_executable(HelloTriangleE3)

target_sources(HelloTriangleE3
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
)

target_link_libraries(HelloTriangleE3
  PRIVATE
    OpenGL::GL
    glfw
    glad
)
