#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Shader.hpp"

class Vertex;
class Camera;
class Light;
class Chunk;

class Terrain {
  private:
    Shader shader;
    Shader terrainGenCompute;
    Shader terrainNormalCompute;
    Shader depthShader;

    unsigned int rockTex;
    unsigned int snowTex;

    std::vector<Chunk *> chunks;

  public:
    Terrain();
    ~Terrain();

    void update(Camera &camera);
    void render(Camera &camera, Light &light, unsigned int depthMap, glm::mat4 &lightSpaceMatrix);
    void renderDepthPass(glm::mat4 &lightSpaceMatrix);

    inline Shader &getShader() {
        return this->shader;
    }
};