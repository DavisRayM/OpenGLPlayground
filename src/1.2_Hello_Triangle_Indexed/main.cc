// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <print>
// clang-format on

/*
 * Window Properties
 */
const char* kWindowTitle = "Hello Triangle Indexed";
const int kWindowWidth = 800;
const int kWindowHeight = 600;

/*
 * Program Settings
 */
enum DrawMode { FILL = GL_FILL, LINE = GL_LINE };
int mode = DrawMode::FILL;
const char* kUsage = R"(
Exit Application by pressing <ESC>.

Switch settings with:
  M - Swap Draw mode between Line and Fill
)";

/*
 * OpenGL Objects
 */
unsigned int VBO, VAO, EBO;

/*
 * Vertex Data
 */
const float kVertices[] = {
    -0.5, -0.5, 0.0,  // bottom left
    -0.5, 0.5,  0.0,  // top left
    0.5,  0.5,  0.0,  // top right
    0.5,  -0.5, 0.0,  // bottom right
};

const unsigned int kIndices[] = {
    0, 1, 2,  // left triangle
    3, 0, 2,  // right triangle
};

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
  color = vec4(0.5, 0.3, 0.1, 1.0);
}
)";

/*
 * Function Declarations
 */
static int Fail(std::string description);
void ResizeCallback(GLFWwindow* window, int width, int height);
static int CreateShaderProgram(const char* vShader, const char* fShader);
static void BufferData();
static void ProcessInputs(GLFWwindow* win);

int main(void) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if (__APPLE__)
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

  GLFWwindow* win =
      glfwCreateWindow(kWindowWidth, kWindowHeight, kWindowTitle, NULL, NULL);
  if (win == NULL) {
    return Fail("Failed to create window.");
  }

  glfwMakeContextCurrent(win);
  glfwSetFramebufferSizeCallback(win, ResizeCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return Fail("Failed to initialize GLAD.");
  }

  unsigned int program = CreateShaderProgram(kVertexShader, kFragmentShader);
  if (program == 0) return Fail("Failed to create shader program.");

  BufferData();

  std::println("{}", kUsage);

  while (!glfwWindowShouldClose(win)) {
    ProcessInputs(win);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, mode);

    glUseProgram(program);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, std::size(kIndices), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(program);

  glfwTerminate();
  return 0;
}

static int Fail(std::string d) {
  std::println(stderr, "{}", d);
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
  unsigned int vertexShader, fragmentShader, program;
  int success;
  char infoLog[512];

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vShader, NULL);
  if (!CompileShader(vertexShader)) {
    return 0;
  }

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fShader, NULL);
  if (!CompileShader(fragmentShader)) {
    return 0;
  }

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
  // Generate objects
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // Bind objects
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
  glEnableVertexAttribArray(0);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kIndices), kIndices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

static void ProcessInputs(GLFWwindow* win) {
  static bool modeSwitchPressed = false;

  if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(win, true);

  if (glfwGetKey(win, GLFW_KEY_M) == GLFW_PRESS) {
    if (!modeSwitchPressed) {
      modeSwitchPressed = true;
      mode = (mode == DrawMode::FILL) ? DrawMode::LINE : DrawMode::FILL;
    }
  } else {
    modeSwitchPressed = false;
  }
}
