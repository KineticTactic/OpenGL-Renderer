#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

class FlyCamera;

class Input {
  private:
    inline static FlyCamera *camera = nullptr;
    inline static GLFWwindow *window = nullptr;
    inline static float lastMouseX = 0.0f;
    inline static float lastMouseY = 0.0f;

  public:
    static void init(GLFWwindow *window, FlyCamera *camera);
    static void processInput(double dt);

    static void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};