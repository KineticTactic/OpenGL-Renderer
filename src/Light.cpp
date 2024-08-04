#include "pch.hpp"
#include "Light.hpp"

#include "Shader.hpp"

Light::Light(glm::vec3 color)
    : position(glm::vec3(0.f)), direction(glm::vec3(-1.f)), color(color), constant(1.0f),
      linear(0.045f), quadratic(0.0075f), intensity(1.0f) {
}

void Light::applyToShader(Shader &shader) {
    // Set w to 0 for directional light and 1 for point light
    shader.setVec4("light.posOrDir", this->isDirectional ? glm::vec4(this->direction, 0.0f)
                                                         : glm::vec4(this->position, 1.0f));
    shader.setVec3("light.color", this->color);
    shader.setFloat("light.intensity", this->intensity);

    if (this->isDirectional)
        return;

    // These uniforms are only for point light attenuation
    shader.setFloat("light.constant", this->constant);
    shader.setFloat("light.linear", this->linear);
    shader.setFloat("light.quadratic", this->quadratic);
}

void Light::setPosition(glm::vec3 position) {
    this->position = position;
    this->isDirectional = false;
}

void Light::setDirection(glm::vec3 direction) {
    this->direction = direction;
    this->isDirectional = true;
}
