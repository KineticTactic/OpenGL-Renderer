#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

class OrbitCamera;

class Input {
  private:
    inline static OrbitCamera *camera = nullptr;
    inline static GLFWwindow *window = nullptr;
    inline static float lastMouseX = 0.0f;
    inline static float lastMouseY = 0.0f;

  public:
    static void init(GLFWwindow *window, OrbitCamera *camera);
    static void processInput();

    static void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
};