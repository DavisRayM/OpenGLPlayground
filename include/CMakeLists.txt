add_library(glad STATIC)

target_sources(glad
  PRIVATE
    glad/glad.c
)

target_include_directories(glad
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
