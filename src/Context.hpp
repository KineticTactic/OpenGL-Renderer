#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

class Context {
  public:
    static GLFWwindow *createWindow();
    static void initOpenGL();
};