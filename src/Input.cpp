#include "Input.hpp"

#include <iostream>

void Input::init(GLFWwindow *window, Camera *camera) {
    glfwSetWindowUserPointer(window, camera);
    glfwSetCursorPosCallback(window, Input::mouseMoveCallback);
    glfwSetScrollCallback(window, Input::scrollCallback);
    Input::camera = camera;
    Input::window = window;
}

void Input::processInput() {
    if (glfwGetKey(Input::window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(Input::window, true);
    }
}

void Input::mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
    double deltaX = xpos - Input::lastMouseX;
    double deltaY = ypos - Input::lastMouseY;

    Input::lastMouseX = xpos;
    Input::lastMouseY = ypos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        Camera *camera = (Camera *)glfwGetWindowUserPointer(window);
        camera->move(glm::vec2(-deltaX, deltaY));
    }
}

void Input::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    Camera *camera = (Camera *)glfwGetWindowUserPointer(window);
    camera->zoom(-yoffset);
}
