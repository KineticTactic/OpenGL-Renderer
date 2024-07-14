#pragma once

#include <glm/glm.hpp>

class Camera {
  private:
    glm::vec3 target;
    glm::mat4 viewMatrix;

    float theta;
    float phi;
    float radius;

  public:
    Camera(glm::vec3 target);

    void move(glm::vec2 offset);
    void zoom(float offset);

    glm::mat4 &getViewMatrix();
    glm::vec3 getPosition();

  private:
    void updateViewMatrix();
};