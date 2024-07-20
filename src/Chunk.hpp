#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/gl.h>

class Vertex;
class OrbitCamera;
class Light;

class Chunk {
  private:
    std::vector<float> heightMap;

    unsigned int textureID;

    glm::vec3 worldPos;

    bool generated = false;
    std::vector<bool> generatedLODs;

    static float chunkSize;
    static float cellSize;
    inline static int heightMapRes = 256;

    inline static GLuint vao = 0;
    inline static GLuint vbo = 0;

  public:
    Chunk(int chunkX, int chunkZ);
    ~Chunk();

    void generate();

    void render(class Shader &shader);

    inline bool isGenerated() {
        return this->generated;
    }

    static void generateBuffers();
    static void deleteBuffers();
};