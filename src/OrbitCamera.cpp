#include "pch.hpp"
#include "OrbitCamera.hpp"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"

OrbitCamera::OrbitCamera(glm::vec3 target) : Camera(1366.0f / 768.0f) {
    this->target = target;
    this->theta = 0.0f;
    this->phi = 0.03f;
    this->radius = 2000.0f;
    this->updateViewMatrix();
    this->updateProjectionMatrix();
}

void OrbitCamera::move(glm::vec3 offset) {
    this->target +=
        glm::vec3(glm::vec4(offset.x, offset.y, offset.z, 0) *
                  glm::rotate(glm::mat4(1.0f), -this->theta, glm::vec3(0.0f, 1.0f, 0.0f)));
    this->updateViewMatrix();
}

void OrbitCamera::orbit(glm::vec2 offset) {
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

void OrbitCamera::zoom(float offset) {
    this->radius += offset / 0.01f;

    if (this->radius < 0.1f) {
        this->radius = 0.1f;
    }

    this->updateViewMatrix();
}

void OrbitCamera::updateViewMatrix() {
    // std::cout << "Position: " << position.x << ", " << position.y << ", " << position.z << ", ";
    // std::cout << "Rotation: " << this->theta << ", " << this->phi << std::endl;

    this->position = this->target + glm::vec3(this->radius * sin(this->theta) * cos(this->phi),
                                              this->radius * sin(this->phi),
                                              this->radius * cos(this->phi) * cos(this->theta));

    this->viewMatrix = glm::lookAt(this->position, this->target, glm::vec3(0.0f, 1.0f, 0.0f));
    this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}
