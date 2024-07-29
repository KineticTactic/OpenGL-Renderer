#include "QuadTree.hpp"

#include <iostream>
#include "Renderable.hpp"
#include "Shader.hpp"

QuadTree::QuadTree(glm::vec2 worldPos, float size, float leafSize)
    : worldPos(worldPos), size(size), leafSize(leafSize) {
}

QuadTree::~QuadTree() {
}

void QuadTree::insert(glm::vec2 target) {
    QuadNode root = {this->worldPos, this->size, false};
    this->recursivelyInsert(target, root);
    // std::cout << "[QUADTREE]: Generated " << this->nodes.size() << " nodes" << std::endl;
    return;
}

void QuadTree::recursivelyInsert(glm::vec2 target, QuadNode node) {
    float distMultiplier = 1.0f;
    float dist = glm::distance(target, node.worldPos);

    if (dist > node.size * distMultiplier || node.size <= this->leafSize) {
        node.isLeafNode = true;
        this->nodes.push_back(node);
        return;
    }
    this->nodes.push_back(node);

    float halfSize = node.size / 2.0f;
    float quartSize = node.size / 4.0f;

    QuadNode c1 = {node.worldPos + glm::vec2(-quartSize, -quartSize), halfSize, false};
    QuadNode c2 = {node.worldPos + glm::vec2(quartSize, -quartSize), halfSize, false};
    QuadNode c3 = {node.worldPos + glm::vec2(-quartSize, quartSize), halfSize, false};
    QuadNode c4 = {node.worldPos + glm::vec2(quartSize, quartSize), halfSize, false};

    this->recursivelyInsert(target, c1);
    this->recursivelyInsert(target, c2);
    this->recursivelyInsert(target, c3);
    this->recursivelyInsert(target, c4);
}

void QuadTree::renderDebug(Renderable &quad, Shader &shader) {
    for (auto &node : this->nodes) {
        if (node.isLeafNode) {
            quad.setPosition(glm::vec3(node.worldPos.x, 0.0f, node.worldPos.y));
            quad.setScale(glm::vec3(node.size, 1.0f, node.size));
            shader.setVec3("objectColor", glm::vec3(0.6f, 0.1f, 0.5f));
            quad.render(shader);
        }
    }
}

void QuadTree::renderDebug(std::vector<QuadNode> nodes, Renderable &quad, Shader &shader) {
    for (auto &node : nodes) {
        if (node.isLeafNode) {
            quad.setPosition(glm::vec3(node.worldPos.x, 0.0f, node.worldPos.y));
            quad.setScale(glm::vec3(node.size, 1.0f, node.size));
            shader.setVec3("objectColor", glm::vec3(0.6f, 0.1f, 0.5f));
            quad.render(shader);
        }
    }
}
