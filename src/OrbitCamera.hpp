#pragma once

#include <glm/glm.hpp>

class OrbitCamera {
  private:
    glm::vec3 target;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 viewProjectionMatrix;

    float theta;
    float phi;
    float radius;

    float aspect;

  public:
    OrbitCamera(glm::vec3 target);

    void move(glm::vec3 offset);
    void orbit(glm::vec2 offset);
    void zoom(float offset);

    void applyToShader(class Shader &shader);

    const glm::mat4 &getViewMatrix();
    const glm::mat4 &getProjectionMatrix();
    glm::vec3 getPosition() const;

    void setAspectRatio(float aspectRatio);

  private:
    void updateViewMatrix();
    void updateProjectionMatrix();
};