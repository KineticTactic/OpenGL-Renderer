#include "Input.hpp"

#include <iostream>
#include "OrbitCamera.hpp"

void Input::init(GLFWwindow *window, OrbitCamera *camera) {
    glfwSetCursorPosCallback(window, Input::mouseMoveCallback);
    glfwSetScrollCallback(window, Input::scrollCallback);
    Input::camera = camera;
    Input::window = window;
}

void Input::processInput() {
    if (glfwGetKey(Input::window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(Input::window, true);
    }

    float speed = 0.3f;
    if (glfwGetKey(Input::window, GLFW_KEY_W) == GLFW_PRESS) {
        Input::camera->move(glm::vec3(0.0f, 0.0f, -speed));
    }
    if (glfwGetKey(Input::window, GLFW_KEY_S) == GLFW_PRESS) {
        Input::camera->move(glm::vec3(0.0f, 0.0f, speed));
    }
    if (glfwGetKey(Input::window, GLFW_KEY_A) == GLFW_PRESS) {
        Input::camera->move(glm::vec3(-speed, 0.0f, 0.0f));
    }
    if (glfwGetKey(Input::window, GLFW_KEY_D) == GLFW_PRESS) {
        Input::camera->move(glm::vec3(speed, 0.0f, 0.0f));
    }
}

void Input::mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
    double deltaX = xpos - Input::lastMouseX;
    double deltaY = ypos - Input::lastMouseY;

    Input::lastMouseX = xpos;
    Input::lastMouseY = ypos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        OrbitCamera *camera = (OrbitCamera *)glfwGetWindowUserPointer(window);
        camera->orbit(glm::vec2(-deltaX, deltaY));
    }
}

void Input::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    OrbitCamera *camera = (OrbitCamera *)glfwGetWindowUserPointer(window);
    camera->zoom(-yoffset);
}
