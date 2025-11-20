#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <print>
#include <toolkit/window.h>

const size_t WINDOW_WIDTH = 800;
const size_t WINDOW_HEIGHT = 600;

// Window resize callback.
void ResizeCallback(GLFWwindow *, int, int);

// Prints error description and return exit code.
int Fail(std::string);

// Process window keyboard inputs.
void ProcessInput(GLFWwindow *);

int main(void) {
  InitGLFW(3, 3, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
  if (window == NULL) {
    return Fail("Failed to create GLFW Window.");
  }

  glfwMakeContextCurrent(window);

  // Load OpenGL function pointers
  if (!InitGLAD()) {
    return Fail("Failed to initialize GLAD.");
  }

  // Register window resize callback
  glfwSetFramebufferSizeCallback(window, ResizeCallback);

  while (!glfwWindowShouldClose(window)) {
    ProcessInput(window);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void ResizeCallback(GLFWwindow *win, int width, int height) {
  glViewport(0, 0, width, height);
}

int Fail(std::string desc) {
  std::println(stderr, "%s", desc);
  glfwTerminate();
  return -1;
}

void ProcessInput(GLFWwindow *win) {
  if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(win, true);
}
