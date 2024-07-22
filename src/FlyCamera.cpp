#include "FlyCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

FlyCamera::FlyCamera(glm::vec3 position) : Camera(1366.0f / 768.0f), pitch(0.0f), yaw(0.0f) {
    this->position = position;
    this->facing = glm::vec3(0.0f, 0.0f, 1.0f);
    this->updateViewMatrix();
}

void FlyCamera::move(glm::vec3 offset) {
    // Move the camera along its forward vector
    this->position += this->facing * offset.z;

    // Move the camera along its right vector (cross product of forward and up)
    glm::vec3 right = glm::normalize(glm::cross(this->facing, glm::vec3(0.0f, 1.0f, 0.0f)));
    this->position += right * offset.x;

    // Move the camera along its up vector
    this->position += glm::vec3(0.0f, 1.0f, 0.0f) * offset.y;

    this->updateViewMatrix();
}

void FlyCamera::rotate(glm::vec2 offset) {
    yaw += offset.x;
    pitch += offset.y;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    direction.y = sin(glm::radians(this->pitch));
    direction.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->facing = glm::normalize(direction);

    this->updateViewMatrix();
}

void FlyCamera::updateViewMatrix() {
    this->viewMatrix =
        glm::lookAt(this->position, this->position + this->facing, glm::vec3(0.0f, 1.0f, 0.0f));
    this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}