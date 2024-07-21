#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera;

class Context {
  public:
    static GLFWwindow *createWindow();
    static void initOpenGL();
    static void setCamera(GLFWwindow *window, Camera &camera);
    static glm::vec2 getFramebufferSize();

  private:
    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
};