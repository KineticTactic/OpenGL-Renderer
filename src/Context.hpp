#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

class OrbitCamera;

class Context {
  public:
    static GLFWwindow *createWindow();
    static void initOpenGL();
    static void setCamera(GLFWwindow *window, OrbitCamera &camera);

  private:
    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
};