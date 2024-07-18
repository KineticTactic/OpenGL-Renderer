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
    std::vector<unsigned int> indices;

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

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