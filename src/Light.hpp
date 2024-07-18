#pragma once

#include <glm/glm.hpp>

class Light {
  private:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;

    bool isDirectional = false;

    float constant;
    float linear;
    float quadratic;

    float intensity;

  public:
    Light(glm::vec3 color);

    void applyToShader(class Shader &shader);

    void setPosition(glm::vec3 position);
    void setDirection(glm::vec3 direction);

    inline glm::vec3 getPosition() {
        return this->position;
    }
    inline glm::vec3 getDirection() {
        return this->direction;
    }
    inline glm::vec3 getColor() {
        return this->color;
    }
    inline void setColor(glm::vec3 color) {
        this->color = color;
    }
    inline void setIntensity(float intensity) {
        this->intensity = intensity;
    }
};