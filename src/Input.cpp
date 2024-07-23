#include "Input.hpp"

#include <iostream>
#include "FlyCamera.hpp"

void Input::init(GLFWwindow *window, FlyCamera *camera) {
    glfwSetCursorPosCallback(window, Input::mouseMoveCallback);
    glfwSetScrollCallback(window, Input::scrollCallback);
    glfwSetKeyCallback(window, Input::keyCallback);
    Input::camera = camera;
    Input::window = window;
}

void Input::processInput(double dt) {

    float speed = 1000.f * dt;
    if (glfwGetKey(Input::window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        speed *= 5;
    }
    if (glfwGetKey(Input::window, GLFW_KEY_W) == GLFW_PRESS) {
        Input::camera->move(glm::vec3(0.0f, 0.0f, speed));
    }
    if (glfwGetKey(Input::window, GLFW_KEY_S) == GLFW_PRESS) {
        Input::camera->move(glm::vec3(0.0f, 0.0f, -speed));
    }
    if (glfwGetKey(Input::window, GLFW_KEY_A) == GLFW_PRESS) {
        Input::camera->move(glm::vec3(-speed, 0.0f, 0.0f));
    }
    if (glfwGetKey(Input::window, GLFW_KEY_D) == GLFW_PRESS) {
        Input::camera->move(glm::vec3(speed, 0.0f, 0.0f));
    }
    if (glfwGetKey(Input::window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        Input::camera->move(glm::vec3(0.0f, speed, 0.0f));
    }
    if (glfwGetKey(Input::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        Input::camera->move(glm::vec3(0.0f, -speed, 0.0f));
    }
}

void Input::mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
    double deltaX = xpos - Input::lastMouseX;
    double deltaY = ypos - Input::lastMouseY;

    Input::lastMouseX = xpos;
    Input::lastMouseY = ypos;

    FlyCamera *camera = (FlyCamera *)glfwGetWindowUserPointer(window);
    float sensitivity = 0.1f;
    camera->rotate(glm::vec2(deltaX * sensitivity, -deltaY * sensitivity));

    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    // camera->orbit(glm::vec2(-deltaX, deltaY));
    // }
}

void Input::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    // FlyCamera *camera = (FlyCamera *)glfwGetWindowUserPointer(window);
    // camera->zoom(-yoffset);
}

bool isWireframe = false;

void Input::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        isWireframe = !isWireframe;
        glPolygonMode(GL_FRONT_AND_BACK, isWireframe ? GL_LINE : GL_FILL);
    }
}
