#pragma once

#include <glm/glm.hpp>

class Camera {
  protected:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 viewProjectionMatrix;
    glm::vec3 position;
    float aspect;

  public:
    Camera(float aspect);

    void applyToShader(class Shader &shader);

    inline const glm::mat4 &getViewMatrix() {
        return this->viewMatrix;
    }
    inline const glm::mat4 &getProjectionMatrix() {
        return this->projectionMatrix;
    }
    inline const glm::mat4 &getViewProjectionMatrix() {
        return this->viewProjectionMatrix;
    }
    inline const glm::vec3 &getPosition() {
        return this->position;
    }
    void setAspectRatio(float aspectRatio);

  protected:
    virtual void updateViewMatrix() = 0;
    void updateProjectionMatrix();
};