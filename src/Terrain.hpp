#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Shader.hpp"

class Vertex;
class OrbitCamera;
class Light;

class Terrain {
  private:
    std::vector<Vertex> vertices;

    unsigned int vao;
    unsigned int vbo;

    Shader shader;
    glm::vec3 worldPos;

  public:
    Terrain();
    ~Terrain();

    void render(OrbitCamera &camera, Light &light);

    inline Shader &getShader() {
        return this->shader;
    }
};