// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <print>

/*
 * Window Properties
 */
const char* kWindowTitle = "Hello Triangle Exercise 1";
const int kWindowWidth = 800;
const int kWindowHeight = 600;

/*
 * Vertex Data
 */
// clang-format off
const float kVertices1[] = {
  -0.8, -0.5, 0.0, // Left
  0.0, -0.5, 0.0, // Right
  -0.4, 0.5, 0.0,  // Top
};
const float kVertices2[] = {
  0.0, -0.5, 0.0, // Left
  0.8, -0.5, 0.0, // Right
  0.4, 0.5, 0.0, // Top
};
// clang-format on
constexpr int kVertexCount1 = std::size(kVertices1) / 3;
constexpr int kVertexCount2 = std::size(kVertices2) / 3;

/*
 * OpenGL Objects
 */
unsigned int VBOs[2], VAOs[2];

/*
 * Shaders
 */
const char* kVertexShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
  gl_Position = vec4(aPos, 1.0);
}
)";
const char* kFragmentShader = R"(
#version 330 core
out vec4 color;

void main() {
  color = vec4(1.0, 0.8, 0.0, 1.0);
1.0f, 0.5f, 0.2f, 1.0f
}
)";
const char* kFragmentShader2 = R"(
#version 330 core
out vec4 color;

void main() {
  color = vec4(1.0, 0.5, 0.2, 1.0);
}
)";

static int Fail(std::string description);
void ResizeCallback(GLFWwindow* window, int width, int height);
static int CreateShaderProgram(const char* vertexShader,
                               const char* fragmentShader);
static void BufferData();
static void ProcessInputs(GLFWwindow* window);

int main() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

  GLFWwindow* win =
      glfwCreateWindow(kWindowWidth, kWindowHeight, kWindowTitle, NULL, NULL);
  if (win == NULL) return Fail("Failed to create window.");

  glfwMakeContextCurrent(win);
  glfwSetFramebufferSizeCallback(win, ResizeCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    return Fail("Failed to initialize GLAD.");

  unsigned int program = CreateShaderProgram(kVertexShader, kFragmentShader);
  if (program == 0) return Fail("Failed to create shader program.");

  BufferData();

  while (!glfwWindowShouldClose(win)) {
    ProcessInputs(win);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, kVertexCount1);

    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, kVertexCount2);

    glBindVertexArray(0);
    glUseProgram(0);

    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glDeleteVertexArrays(2, VAOs);
  glDeleteBuffers(2, VBOs);
  glDeleteProgram(program);

  glfwTerminate();
  return 0;
}

static int Fail(std::string m) {
  std::println(stderr, "{}", m);
  glfwTerminate();
  return -1;
}

void ResizeCallback(GLFWwindow* win, int w, int h) { glViewport(0, 0, w, h); }

static bool CompileShader(unsigned int sID) {
  int success;
  char infoLog[512];

  glCompileShader(sID);

  glGetShaderiv(sID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(sID, 512, NULL, infoLog);
    std::println(stderr, "Failed to compile shader: {}", infoLog);
  }

  return success;
}

static int CreateShaderProgram(const char* vShader, const char* fShader) {
  int success;
  unsigned int vertexShader, fragmentShader, program;
  char infoLog[512];

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vShader, NULL);
  if (!CompileShader(vertexShader)) return 0;

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fShader, NULL);
  if (!CompileShader(fragmentShader)) return 0;

  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::println(stderr, "Failed to link shader program: {}", infoLog);
    return 0;
  }

  return program;
}

static void BufferData() {
  glGenVertexArrays(2, VAOs);
  glGenBuffers(2, VBOs);

  glBindVertexArray(VAOs[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

  glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices1), kVertices1, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAOs[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);

  glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices2), kVertices2, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

static void ProcessInputs(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
