#include <glad/glad.h>

#include "window.h"

void InitGLFW(int major, int minor, int prof) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, prof);

#if defined(__APPLE__)
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif
}

bool InitGLAD() { return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); }
