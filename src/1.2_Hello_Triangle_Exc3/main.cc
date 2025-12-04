// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <print>
// clang-format on

/*
 * Window Properties
 */
const size_t kWindowWidth = 800;
const size_t kWindowHeight = 600;
const char* kWindowTitle = "Multi Shader";

/*
 * OpenGL Pointers
 */
unsigned int VAO, VBO, EBO;

/*
 * Vertex Data
 */
// clang-format off
const float kVertices[] = {
  -0.8, -0.5, 0.0, // Left 1
  -0.4, 0.5, 0.0,  // Top 1
  0.0, -0.5, 0.0, // Right 1 | Left 2
  0.8, -0.5, 0.0, // Right 2
  0.4, 0.5, 0.0, // Top 2
};
const unsigned int kIndices[] = {
  0, 1, 2,
  2, 3, 4,
};
// clang-format on

/*
 * Shader Source
 */
const char* kVertexShader = R"(
#version 330 core
layout ( location = 0 ) in vec3 aPos;

void main() {
  gl_Position = vec4(aPos, 1.0);
}
)";

const char* kFragmentShader1 = R"(
#version 330 core
out vec4 color;

void main() {
  color = vec4(1.0, 1.0, 0.0, 1.0);
}
)";

const char* kFragmentShader2 = R"(
#version 330 core
out vec4 color;

void main() {
  color = vec4(1.0, 0.2, 0.8, 1.0);
}
)";

static int Fail(const char* desc);
static void Resize(GLFWwindow* window, int width, int height);
static void ProcessInput(GLFWwindow* window);
static void BufferData();
static unsigned int CompileShaderProgram(const char* vShader,
                                         const char* fShader);

int main(void) {
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
  glfwSetFramebufferSizeCallback(win, Resize);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    return Fail("Failed to initialize GLAD.");

  unsigned int program1 = CompileShaderProgram(kVertexShader, kFragmentShader1);
  if (program1 == 0) return Fail("Failed to create shader program.");
  unsigned int program2 = CompileShaderProgram(kVertexShader, kFragmentShader2);
  if (program2 == 0) return Fail("Failed to create shader program.");
  BufferData();

  while (!glfwWindowShouldClose(win)) {
    ProcessInput(win);
    glClearColor(1.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program1);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    glUseProgram(program2);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,
                   (void*)(3 * sizeof(GLuint)));

    glBindVertexArray(0);
    glUseProgram(0);

    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(program1);
  glDeleteProgram(program2);

  glfwTerminate();
  return 0;
}

static int Fail(const char* desc) {
  std::println(stderr, "{}", desc);
  glfwTerminate();
  return 1;
}

static void Resize(GLFWwindow* win, int w, int h) { glViewport(0, 0, w, h); }

static void ProcessInput(GLFWwindow* win) {
  if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(win, GLFW_TRUE);
}

static void BufferData() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
  glEnableVertexAttribArray(0);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kIndices), kIndices,
               GL_STATIC_DRAW);
}

static bool CompileShader(unsigned int id) {
  int success;
  char infoLog[512];
  glCompileShader(id);

  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id, 512, NULL, infoLog);
    std::println(stderr, "Failed to compile shader: {}", infoLog);
    return false;
  }

  return true;
}

static unsigned int CompileShaderProgram(const char* vertexShader,
                                         const char* fragShader) {
  unsigned int vShaderID, fShaderID;
  int success;
  char infoLog[512];

  vShaderID = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShaderID, 1, &vertexShader, NULL);
  if (!CompileShader(vShaderID)) return 0;

  fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShaderID, 1, &fragShader, NULL);

  if (!CompileShader(fShaderID)) return 0;

  unsigned int program = glCreateProgram();
  glAttachShader(program, vShaderID);
  glAttachShader(program, fShaderID);
  glLinkProgram(program);

  glDeleteShader(vShaderID);
  glDeleteShader(fShaderID);

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::println(stderr, "Failed to link program: {}", infoLog);
    return 0;
  }

  return program;
}
