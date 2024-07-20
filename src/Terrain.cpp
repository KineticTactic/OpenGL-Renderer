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
             "shaders/terrain.frag") {
    std::vector<IntCoords> coords = Spiral(20, 20);
    for (auto &coord : coords) {
        this->chunks.push_back(new Chunk(coord.x, coord.y));
    }

    // TODO: Put into static method
    Chunk::generateBuffers();
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
    for (auto &chunk : this->chunks) {
        if (!chunk->isGenerated()) {
            chunk->generate();
            break;
        }
    }
}

void Terrain::render(OrbitCamera &camera, Light &light) {
    this->shader.use();
    camera.applyToShader(this->shader);
    light.applyToShader(this->shader);
    shader.setMat4("view", camera.getViewMatrix());

    for (auto &chunk : this->chunks) {
        chunk->render(this->shader);
    }
}
