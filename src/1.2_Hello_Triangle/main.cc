// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <print>
#include <string>

// clang-format on

/*
 * Window Properties
 */
const int kWindowWidth = 800;
const int kWindowHeight = 600;

/*
 * Vertex Data
 */
const float kVertices[] = {
    -0.5, -0.5, 0.0, 0.5, -0.5, 0.0, 0.0, 0.5, 0.0,
};

/*
 * OpenGL Objects
 */
unsigned int VAO, VBO, program;

/*
 * Shaders
 */
const char* kVertexShader = R"(
#version 330 core
layout ( location = 0 ) in vec3 aPos;

void main() {
  gl_Position = vec4(aPos, 1.0);
}
)";

const char* kFragmentShader = R"(
#version 330 core
out vec4 color;

void main() {
  color = vec4(1.0, 0.0, 0.0, 1.0);
}
)";

static int Fail(std::string desc);
static unsigned int CreateProgramWithShaders(const char* vShader,
                                             const char* fShader);
static void BufferData();
static void ProcessInput(GLFWwindow* window);
void ResizeCallback(GLFWwindow* win, int width, int height);

int main(void) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

  GLFWwindow* win = glfwCreateWindow(kWindowWidth, kWindowHeight,
                                     "Hello Triangle", NULL, NULL);
  if (win == NULL) {
    return Fail("ERROR: Failed to create window.");
  }

  glfwMakeContextCurrent(win);
  glfwSetFramebufferSizeCallback(win, ResizeCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return Fail("ERROR: Failed to initialize GLAD");
  }

  program = CreateProgramWithShaders(kVertexShader, kFragmentShader);
  if (program == 0) {
    return Fail("ERROR: Failed to create shader program");
  }

  BufferData();

  while (!glfwWindowShouldClose(win)) {
    ProcessInput(win);

    glClearColor(0.5f, 0.3f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glUseProgram(0);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(program);

  glfwTerminate();
  return 0;
}

int Fail(std::string desc) {
  std::println(stderr, "{}", desc);
  glfwTerminate();
  return -1;
}

bool CompileShader(unsigned int shaderID) {
  int success;
  char infoLog[512];
  glCompileShader(shaderID);

  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
    std::println(stderr, "ERROR::SHADER: {}", infoLog);
  }

  return success;
}

unsigned int CreateProgramWithShaders(const char* vShader,
                                      const char* fShader) {
  int success;
  char infoLog[512];

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vShader, NULL);
  if (!CompileShader(vertexShader)) {
    return 0;
  }

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fShader, NULL);
  if (!CompileShader(fragmentShader)) {
    return 0;
  }

  unsigned int prog = glCreateProgram();
  glAttachShader(prog, vertexShader);
  glAttachShader(prog, fragmentShader);
  glLinkProgram(prog);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glGetProgramiv(prog, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(prog, 512, NULL, infoLog);
    std::println(stderr, "ERROR: Failed to link program '{}'", infoLog);

    return 0;
  }

  return prog;
}

static void BufferData() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

static void ProcessInput(GLFWwindow* win) {
  if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(win, true);
}

void ResizeCallback(GLFWwindow* win, int width, int height) {
  glViewport(0, 0, width, height);
}
