#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "GrassRenderer.hpp"

class Vertex;
class Camera;
class Light;
class Chunk;
class QuadNode;

class Terrain {
  private:
    Shader shader;
    Shader terrainGenCompute;
    Shader terrainNormalCompute;
    Shader depthShader;

    unsigned int rockTex;
    unsigned int snowTex;
    unsigned int grassFieldTex;

    std::vector<QuadNode> nodes;
    std::vector<Chunk *> chunks;

    GrassRenderer grass;

    inline static float MIN_CHUNK_SIZE = 255.0f;
    inline static float TERRAIN_SIZE = 100000.0f;

  public:
    Terrain();
    ~Terrain();

    void update(Camera &camera);
    void render(Camera &camera, Light &light, unsigned int depthMap, glm::mat4 &lightSpaceMatrix);
    void renderDepthPass(glm::mat4 &lightSpaceMatrix);

    void clearTerrain();

    inline Shader &getShader() {
        return this->shader;
    }

    /// TODO: remove
    inline std::vector<QuadNode> getNodes() {
        return this->nodes;
    }
};