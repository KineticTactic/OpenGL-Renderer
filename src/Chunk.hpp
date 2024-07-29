#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/gl.h>

class Vertex;
class Camera;
class Light;
class Shader;
class GrassRenderer;

class Chunk {
    friend class Terrain;

  private:
    std::vector<float> heightMap;

    unsigned int heightMapID;

    glm::vec3 worldPos;
    float chunkSize;

    bool generated = false;

    inline static GLuint vao = 0;
    inline static GLuint vbo = 0;

  public:
    inline static int subdivisions = 8;
    inline static int heightMapRes = 256;

    Chunk(glm::vec3 worldPos, float chunkSize);
    ~Chunk();

    void generate(Shader &terrainGenCompute, Shader &terrainNormalCompute, GrassRenderer &grass);

    void render(Shader &shader);

    inline bool isGenerated() {
        return this->generated;
    }

    static void generateBuffers();
    static void deleteBuffers();
};