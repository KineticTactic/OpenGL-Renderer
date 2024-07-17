#include "OrbitCamera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Shader.hpp"

OrbitCamera::OrbitCamera(glm::vec3 target) {
    this->target = target;
    this->theta = 0.0f;
    this->phi = 0.0f;
    this->radius = 15.0f;
    this->aspect = 1366.0f / 768.0f;
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
    this->radius += offset / 1.0f;

    if (this->radius < 0.1f) {
        this->radius = 0.1f;
    }

    this->updateViewMatrix();
}

void OrbitCamera::applyToShader(Shader &shader) {
    shader.setMat4("viewProjection", this->viewProjectionMatrix);
    shader.setVec3("viewPos", this->getPosition());
}

const glm::mat4 &OrbitCamera::getViewMatrix() {
    return this->viewMatrix;
}

const glm::mat4 &OrbitCamera::getProjectionMatrix() {
    return this->projectionMatrix;
}

glm::vec3 OrbitCamera::getPosition() const {
    return this->target + glm::vec3(this->radius * sin(this->theta) * cos(this->phi),
                                    this->radius * sin(this->phi),
                                    this->radius * cos(this->phi) * cos(this->theta));
}

void OrbitCamera::setAspectRatio(float aspectRatio) {
    this->aspect = aspectRatio;
    this->updateProjectionMatrix();
}

void OrbitCamera::updateViewMatrix() {
    glm::vec3 position = this->getPosition();

    // std::cout << "Position: " << position.x << ", " << position.y << ", " << position.z << ", ";
    // std::cout << "Rotation: " << this->theta << ", " << this->phi << std::endl;

    this->viewMatrix = glm::lookAt(position, this->target, glm::vec3(0.0f, 1.0f, 0.0f));
    this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}

void OrbitCamera::updateProjectionMatrix() {
    this->projectionMatrix = glm::perspective(glm::radians(45.0f), this->aspect, 0.1f, 1000.0f);
    this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}
