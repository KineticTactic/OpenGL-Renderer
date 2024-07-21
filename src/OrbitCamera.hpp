#pragma once

#include "Camera.hpp"

class OrbitCamera : public Camera {
  private:
    glm::vec3 target;

    float theta;
    float phi;
    float radius;

  public:
    OrbitCamera(glm::vec3 target);

    void move(glm::vec3 offset);
    void orbit(glm::vec2 offset);
    void zoom(float offset);

  private:
    void updateViewMatrix() override;
};