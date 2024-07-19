#include "Chunk.hpp"

#include <FastNoise/FastNoiseLite.h>
#include <glad/gl.h>
#include <iostream>
#include <GLFW/glfw3.h>

#include "Vertex.hpp"
#include "Shader.hpp"

float Chunk::chunkSize = 200.0f;
float Chunk::cellSize = 1.0f;

Chunk::Chunk(int chunkX, int chunkZ)
    : worldPos({chunkX * Chunk::chunkSize, 0.0f, chunkZ * Chunk::chunkSize}) {
}

Chunk::~Chunk() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
}

void Chunk::generate() {
    double start = glfwGetTime();
    int gridSize = Chunk::chunkSize / Chunk::cellSize;

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(0.01f);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetFractalLacunarity(3.5f);
    noise.SetFractalGain(0.15f);

    FastNoiseLite noise2;
    noise2.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise2.SetFrequency(0.0015f);
    noise2.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise2.SetFractalLacunarity(1.0f);

    std::cout << "[CHUNK]: Generating chunk..." << std::endl;

    for (int j = 0; j < gridSize; j++) {
        for (int i = 0; i < gridSize; i++) {
            float height = 15;
            float height2 = 200;
            float y =
                noise.GetNoise(i * cellSize + worldPos.x, j * cellSize + worldPos.z) * height +
                noise2.GetNoise(i * cellSize + worldPos.x, j * cellSize + worldPos.z) * height2;
            glm::vec3 v = glm::vec3(i * cellSize, y, j * cellSize);

            float dx = 0.1f;
            glm::vec3 u1 = glm::vec3(
                i * cellSize + dx,
                noise.GetNoise(i * cellSize + worldPos.x + dx, j * cellSize + worldPos.z) * height +
                    noise2.GetNoise(i * cellSize + worldPos.x + dx, j * cellSize + worldPos.z) *
                        height2,
                j * cellSize);
            glm::vec3 u2 = glm::vec3(
                i * cellSize,
                noise.GetNoise(i * cellSize + worldPos.x, j * cellSize + worldPos.z + dx) * height +
                    noise2.GetNoise(i * cellSize + worldPos.x, j * cellSize + worldPos.z + dx) *
                        height2,
                j * cellSize + dx);
            glm::vec3 t1 = u1 - v;
            glm::vec3 t2 = u2 - v;
            glm::vec3 n = glm::normalize(glm::cross(t1, t2)) * -1.f;

            glm::vec3 col = glm::vec3(0.5f, 0.5f + (y / (height + height2) + 1) / 4, 0.5f);
            if (y / (height + height2) <= -0.5f) {
                col = glm::vec3(0.5f, 0.63f, 0.5f + (-y / (height + height2) - 0.f));
            }
            this->vertices.push_back({v, n, col});
        }
    }

    for (int j = 0; j < gridSize - 1; j++) {
        for (int i = 0; i < gridSize - 1; i++) {
            int idx = j * gridSize + i;
            indices.push_back(idx);
            indices.push_back(idx + gridSize);
            indices.push_back(idx + 1);

            indices.push_back(idx + 1);
            indices.push_back(idx + gridSize);
            indices.push_back(idx + gridSize + 1);
        }
    }

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), // position
                          (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), // normal
                          (void *)sizeof(glm::vec3));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), // color
                          (void *)(2 * sizeof(glm::vec3)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &this->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(),
                 GL_STATIC_DRAW);

    this->generated = true;
    double time = glfwGetTime() - start;
    std::cout << "[CHUNK]: Chunk generated: " << this->vertices.size() << " vertices, "
              << time * 1000 << "ms" << std::endl;
}

void Chunk::render(class Shader &shader) {
    if (!this->generated)
        return;

    shader.setVec3("worldPos", this->worldPos);

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
}

float Chunk::heightFunc(glm::vec2 pos) {
}
