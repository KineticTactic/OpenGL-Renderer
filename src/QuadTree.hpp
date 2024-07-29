#pragma once

#include <glm/glm.hpp>
#include <vector>

struct QuadNode {
    glm::vec2 worldPos;
    float size;
    bool isLeafNode = false;
    // TODO: Use smth else other than string
    // std::string id;
};

class QuadTree {
    friend class Terrain;

  private:
    std::vector<QuadNode> nodes;

    glm::vec2 worldPos;
    float size;
    float leafSize;

  public:
    QuadTree(glm::vec2 worldPos, float size, float leafSize);
    ~QuadTree();

    void insert(glm::vec2 target);

    void renderDebug(class Renderable &quad, class Shader &shader);
    static void renderDebug(std::vector<QuadNode> nodes, Renderable &quad, Shader &shader);

  private:
    void recursivelyInsert(glm::vec2 target, QuadNode node);
};
