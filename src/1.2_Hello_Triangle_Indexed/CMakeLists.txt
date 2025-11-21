add_executable(HelloTriangleIndexed)

target_sources(HelloTriangleIndexed
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
)

target_link_libraries(HelloTriangleIndexed
  PRIVATE
    OpenGL::GL
    glfw
    glad
)
