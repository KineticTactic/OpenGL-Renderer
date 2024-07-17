#pragma once

#include <glm/glm.hpp>

class Camera {
  private:
    glm::vec3 target;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 viewProjectionMatrix;

    float theta;
    float phi;
    float radius;

  public:
    Camera(glm::vec3 target);

    void move(glm::vec2 offset);
    void zoom(float offset);

    void applyToShader(class Shader &shader);

    const glm::mat4 &getViewMatrix();
    const glm::mat4 &getProjectionMatrix();
    glm::vec3 getPosition() const;

  private:
    void updateViewMatrix();
    void updateProjectionMatrix();
};