#pragma once
#include <GLFW/glfw3.h>
#include <stdbool.h>

// Initializes GLFW and sets appropriate OpenGL Version and Profile
// hints.
void InitGLFW(int glVersionMajor, int glVersionMinor, int profile);

// Initializes GLAD
bool InitGLAD();
