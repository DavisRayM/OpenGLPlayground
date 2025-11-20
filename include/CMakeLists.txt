add_library(glad STATIC)

target_sources(glad
  PRIVATE
    glad/glad.c

  PUBLIC
    FILE_SET headers
    TYPE HEADERS
    BASE_DIRS
      ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/glad/glad.h
      ${CMAKE_CURRENT_SOURCE_DIR}/KHR/khrplatform.h
)

add_library(toolkit STATIC)

target_sources(toolkit
  PRIVATE
    toolkit/window.cc

  PUBLIC
    FILE_SET headers
    TYPE HEADERS
    BASE_DIRS
      ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/toolkit/window.h
)

target_link_libraries(toolkit
  PRIVATE
    glfw
)
