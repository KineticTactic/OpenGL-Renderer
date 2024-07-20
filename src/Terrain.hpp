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
    Shader compute;
    Shader depthShader;

    std::vector<Chunk *> chunks;

  public:
    Terrain();
    ~Terrain();

    void update(OrbitCamera &camera);
    void render(OrbitCamera &camera, Light &light, unsigned int depthMap,
                glm::mat4 &lightSpaceMatrix);
    void renderDepthPass(glm::mat4 &lightSpaceMatrix);

    inline Shader &getShader() {
        return this->shader;
    }
};