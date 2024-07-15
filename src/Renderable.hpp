#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Shader.hpp"

class Renderable {
  private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int vertexCount;
    glm::mat4 modelMatrix;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

  public:
    Renderable(std::vector<float> const &vertexData);
    ~Renderable();

    void render(Shader &shader);

    inline glm::vec3 getPosition() {
        return this->position;
    }

    inline glm::vec3 getRotation() {
        return this->rotation;
    }

    inline glm::vec3 getScale() {
        return this->scale;
    }

    inline void setPosition(glm::vec3 position) {
        this->position = position;
        this->updateModelMatrix();
    }

    inline void setRotation(glm::vec3 rotation) {
        this->rotation = rotation;
        this->updateModelMatrix();
    }

    inline void setScale(glm::vec3 scale) {
        this->scale = scale;
        this->updateModelMatrix();
    }

  private:
    void updateModelMatrix();
};