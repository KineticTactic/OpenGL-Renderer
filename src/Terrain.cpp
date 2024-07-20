#include "Terrain.hpp"

#include <glad/gl.h>
#include <FastNoise/FastNoiseLite.h>
#include <iostream>
#include <algorithm>

#include "Vertex.hpp"
#include "Shader.hpp"
#include "OrbitCamera.hpp"
#include "Light.hpp"
#include "Chunk.hpp"

struct IntCoords {
    int x;
    int y;
};

std::vector<IntCoords> Spiral(int X, int Y) {
    int x, y, dx, dy;
    x = y = dx = 0;
    dy = -1;
    int t = std::max(X, Y);
    int maxI = t * t;
    std::vector<IntCoords> coords;
    for (int i = 0; i < maxI; i++) {
        if ((-X / 2 <= x) && (x <= X / 2) && (-Y / 2 <= y) && (y <= Y / 2)) {
            coords.push_back({x, y});
        }
        if ((x == y) || ((x < 0) && (x == -y)) || ((x > 0) && (x == 1 - y))) {
            t = dx;
            dx = -dy;
            dy = t;
        }
        x += dx;
        y += dy;
    }
    return coords;
}

Terrain::Terrain()
    : shader("shaders/terrain.vert", "shaders/terrain.tesc", "shaders/terrain.tese",
             "shaders/terrain.frag"),
      compute("shaders/TerrainGen.comp"),
      depthShader("shaders/terrain.vert", "shaders/TerrainDepth.tesc", "shaders/TerrainDepth.tese",
                  "shaders/TerrainDepth.frag") {
    std::vector<IntCoords> coords = Spiral(20, 20);
    for (auto &coord : coords) {
        this->chunks.push_back(new Chunk(coord.x, coord.y));
    }

    // TODO: Put into static method
    Chunk::generateBuffers();
    this->shader.use();
    this->shader.setInt("heightMapRes", Chunk::heightMapRes - 2);
    this->shader.setFloat("chunkSize", Chunk::chunkSize);
}

Terrain::~Terrain() {
    for (auto &chunk : this->chunks) {
        delete chunk;
    }
    // TODO: Put into static method
    Chunk::deleteBuffers();
}

void Terrain::update(OrbitCamera &camera) {

    // bool isCameraOnChunk = false;
    int numPerFrame = 10;
    int generated = 0;
    for (auto &chunk : this->chunks) {
        if (!chunk->isGenerated()) {
            chunk->generate(this->compute);
            generated++;
        }
        if (generated >= numPerFrame) {
            break;
        }
    }
}

void Terrain::render(OrbitCamera &camera, Light &light, unsigned int depthMap,
                     glm::mat4 &lightSpaceMatrix) {
    this->shader.use();
    camera.applyToShader(this->shader);
    light.applyToShader(this->shader);
    shader.setMat4("view", camera.getViewMatrix());
    shader.setInt("shadowMap", 1);
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    for (auto &chunk : this->chunks) {
        chunk->render(this->shader);
    }
}

void Terrain::renderDepthPass(glm::mat4 &lightSpaceMatrix) {
    this->depthShader.use();
    this->depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    for (auto &chunk : this->chunks) {
        chunk->render(this->depthShader);
    }
}
