#include <glad/glad.h>
//
#include <GLFW/glfw3.h>

#include <print>

const size_t kWindowWidth = 800;
const size_t kWindowHeight = 600;

// Triangle vertex positions in Normalized Device Coordinates (range: -1.0
// to 1.0)
const float kVertices[] = {
    -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
};

// OpenGL Object IDs
unsigned int VBO;             // Vertex Buffer Objects ID
unsigned int vertexShader;    // Vertex shader ID
unsigned int fragmentShader;  // Fragment/Pixel shader ID
unsigned int shaderProgram;   // Shader Programm ID
unsigned int VAO;             // Vertex Array Object

// SHADERS
const char* kVertexShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
  gl_Position = vec4(aPos, 1.0f);
}
)";
const char* kFragmentShader = R"(
#version 330 core
out vec4 color;

void main() {
  color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";

void ResizeCallback(GLFWwindow*, int, int);

void ProcessInput(GLFWwindow*);

bool CreateVertexShader();

bool CreateFragmentShader();

bool CreateLinkShaderProgram();

bool CompileShader(unsigned int, std::string, const char**);

void BufferData();

void Display();

int main(void) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(kWindowWidth, kWindowHeight,
                                        "Hello Triangle", NULL, NULL);
  if (window == NULL) {
    std::println(stderr, "Failed to create window.");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  // Register resize callback
  glfwSetFramebufferSizeCallback(window, ResizeCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::println(stderr, "Failed to initialize GLAD.");
    glfwTerminate();
    return -1;
  }

  // Create Shader program
  CreateLinkShaderProgram();
  // Generate, bind and fill vertex buffer object.
  BufferData();

  while (!glfwWindowShouldClose(window)) {
    ProcessInput(window);

    // Render
    Display();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Cleanup
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);
  glfwTerminate();
  return 0;
}

void ResizeCallback(GLFWwindow* win, int width, int height) {
  glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* win) {
  if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(win, true);
}

bool CreateVertexShader() {
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  return CompileShader(vertexShader, "Vertex", &kVertexShader);
}

bool CreateFragmentShader() {
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  return CompileShader(fragmentShader, "Fragment", &kFragmentShader);
}

bool CompileShader(unsigned int id, std::string desc, const char** src) {
  int success;
  char infoLog[512];

  glShaderSource(id, 1, src, NULL);
  glCompileShader(id);

  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id, 512, NULL, infoLog);
    std::println(stderr, "Failed to compile {} shader: {}", desc, infoLog);
    glfwTerminate();
  }

  return success;
}

bool CreateLinkShaderProgram() {
  int success;
  char infoLog[512];
  CreateVertexShader();
  CreateFragmentShader();

  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::println("Failed to link shader program: {}", infoLog);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  return success;
}

void BufferData() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // Bind vertex array first.
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // Static Draw: Copy to GPU once, used many times
  // Stream Draw: Copy to GPU once, used a couple of times
  // Dynamic Draw: Copy to GPU multiple times, used many times
  glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
}

void Display() {
  glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
