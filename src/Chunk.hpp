#pragma once

#include <vector>
#include <glm/glm.hpp>

class Vertex;
class OrbitCamera;
class Light;

class Chunk {
  private:
    /// TODO: Do i really need to store these?
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

    glm::vec3 worldPos;

    static float chunkSize;
    static float cellSize;

    bool generated = false;

  public:
    Chunk(int chunkX, int chunkZ);
    ~Chunk();

    void generate();

    void render(class Shader &shader);

    inline bool isGenerated() {
        return this->generated;
    }

    static float heightFunc(glm::vec2 pos);
};