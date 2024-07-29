#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"

Camera::Camera(float aspect) : aspect(aspect) {
    this->updateProjectionMatrix();
}

void Camera::applyToShader(Shader &shader) {
    shader.setMat4("viewProjection", this->viewProjectionMatrix);
    shader.setVec3("viewPos", this->getPosition());
}

void Camera::setAspectRatio(float aspectRatio) {
    this->aspect = aspectRatio;
    this->updateProjectionMatrix();
}

void Camera::updateProjectionMatrix() {
    this->projectionMatrix = glm::perspective(glm::radians(45.0f), this->aspect, 1.f, 60000.0f);
    this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}
