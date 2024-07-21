#pragma once

#include "Camera.hpp"

class FlyCamera : public Camera {
  private:
    float pitch, yaw;
    glm::vec3 facing;

  public:
    FlyCamera(glm::vec3 position);

    void move(glm::vec3 offset);
    void rotate(glm::vec2 offset);
    void updateViewMatrix() override;
};