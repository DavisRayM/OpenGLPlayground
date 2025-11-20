add_executable(HelloWinClear)

target_sources(HelloWinClear
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
)

target_link_libraries(HelloWinClear
  PRIVATE
    OpenGL::GL
    glfw
    glad
    toolkit
)
