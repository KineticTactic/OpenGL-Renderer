#include "Light.hpp"

Light::Light(glm::vec3 position, glm::vec3 color) : position(position), color(color) {
}

void Light::applyToShader(Shader &shader) {
    shader.setVec3("light.pos", this->position);
    shader.setVec3("light.color", this->color);
}