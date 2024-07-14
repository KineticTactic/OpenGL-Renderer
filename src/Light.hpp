#pragma once

#include <glm/glm.hpp>

#include "Shader.hpp"

class Light {
  private:
    glm::vec3 position;
    glm::vec3 color;

  public:
    Light(glm::vec3 position, glm::vec3 color);

    void applyToShader(Shader &shader);

    inline glm::vec3 getPosition() {
        return this->position;
    }
    inline glm::vec3 getColor() {
        return this->color;
    }

    inline void setPosition(glm::vec3 position) {
        this->position = position;
    }

    inline void setColor(glm::vec3 color) {
        this->color = color;
    }
};