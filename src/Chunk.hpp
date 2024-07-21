#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/gl.h>

class Vertex;
class Camera;
class Light;
class Shader;

class Chunk {
  private:
    std::vector<float> heightMap;

    unsigned int heightMapID;
    unsigned int normalID;

    glm::vec3 worldPos;

    bool generated = false;

    inline static GLuint vao = 0;
    inline static GLuint vbo = 0;

  public:
    inline static float chunkSize = 254.f;
    inline static int subdivisions = 8;
    inline static int heightMapRes = 256;

    Chunk(int chunkX, int chunkZ);
    ~Chunk();

    void generate(Shader &compute);

    void render(Shader &shader);

    inline bool isGenerated() {
        return this->generated;
    }

    static void generateBuffers();
    static void deleteBuffers();
};