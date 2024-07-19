#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Shader.hpp"

class Vertex;
class OrbitCamera;
class Light;
class Chunk;

class Terrain {
  private:
    Shader shader;

    std::vector<Chunk *> chunks;

  public:
    Terrain();
    ~Terrain();

    void update(OrbitCamera &camera);
    void render(OrbitCamera &camera, Light &light);

    inline Shader &getShader() {
        return this->shader;
    }
};