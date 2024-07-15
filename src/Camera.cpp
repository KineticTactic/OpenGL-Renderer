#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Shader.hpp"

Camera::Camera(glm::vec3 target) {
    this->target = target;
    this->theta = 0.0f;
    this->phi = 0.0f;
    this->radius = 15.0f;
    this->updateViewMatrix();
    this->updateProjectionMatrix();
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
    this->radius += offset / 1.0f;

    if (this->radius < 0.1f) {
        this->radius = 0.1f;
    }

    this->updateViewMatrix();
}

void Camera::applyToShader(Shader &shader) {
    shader.setMat4("viewProjection", this->viewProjectionMatrix);
    shader.setVec3("viewPos", this->getPosition());
}

glm::mat4 &Camera::getViewMatrix() {
    return this->viewMatrix;
}

glm::vec3 Camera::getPosition() const {
    return glm::vec3(this->radius * sin(this->theta) * cos(this->phi),
                     this->radius * sin(this->phi),
                     this->radius * cos(this->phi) * cos(this->theta));
}

void Camera::updateViewMatrix() {
    glm::vec3 position = this->getPosition();

    // std::cout << "Position: " << position.x << ", " << position.y << ", " << position.z << ", ";
    // std::cout << "Rotation: " << this->theta << ", " << this->phi << std::endl;

    this->viewMatrix = glm::lookAt(position, this->target, glm::vec3(0.0f, 1.0f, 0.0f));
    this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}

void Camera::updateProjectionMatrix() {
    this->projectionMatrix = glm::perspective(glm::radians(45.0f), 1366.0f / 768.0f, 0.1f, 100.0f);
    this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}
