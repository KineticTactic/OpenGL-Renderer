#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Camera::Camera(glm::vec3 target) {
    this->target = target;
    this->theta = 0.0f;
    this->phi = 0.0f;
    this->radius = 5.0f;
    this->updateViewMatrix();
}

void Camera::move(glm::vec2 offset) {
    this->theta += offset.x / 100.0f;
    this->phi += offset.y / 100.0f;

    if (this->phi > 3.14f / 2.0f) {
        this->phi = 3.14f / 2.0f;
    }

    if (this->phi < -3.14f / 2.0f) {
        this->phi = -3.14f / 2.0f;
    }

    this->updateViewMatrix();
}

void Camera::zoom(float offset) {
    this->radius += offset / 5.0f;

    if (this->radius < 0.1f) {
        this->radius = 0.1f;
    }

    this->updateViewMatrix();
}

glm::mat4 &Camera::getViewMatrix() {
    return this->viewMatrix;
}

glm::vec3 Camera::getPosition() {
    return glm::vec3(this->radius * sin(this->theta) * cos(this->phi),
                     this->radius * sin(this->phi),
                     this->radius * cos(this->phi) * cos(this->theta));
}

void Camera::updateViewMatrix() {
    glm::vec3 position = this->getPosition();

    // std::cout << "Position: " << position.x << ", " << position.y << ", " << position.z << ", ";
    // std::cout << "Rotation: " << this->theta << ", " << this->phi << std::endl;

    this->viewMatrix = glm::lookAt(position, this->target, glm::vec3(0.0f, 1.0f, 0.0f));
}
