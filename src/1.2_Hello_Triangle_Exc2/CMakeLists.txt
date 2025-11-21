add_executable(HelloTriangleE2)

target_sources(HelloTriangleE2
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
)

target_link_libraries(HelloTriangleE2
  PRIVATE
    OpenGL::GL
    glfw
    glad
)
