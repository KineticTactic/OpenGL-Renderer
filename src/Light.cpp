#include "Light.hpp"

#include "Shader.hpp"

Light::Light(glm::vec3 position, glm::vec3 color)
    : position(position), color(color), constant(1.0f), linear(0.045f), quadratic(0.0075f) {
}

void Light::applyToShader(Shader &shader) {
    shader.setVec3("light.pos", this->position);
    shader.setVec3("light.color", this->color);

    shader.setFloat("light.constant", this->constant);
    shader.setFloat("light.linear", this->linear);
    shader.setFloat("light.quadratic", this->quadratic);
}