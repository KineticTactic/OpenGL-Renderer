#include "Terrain.hpp"

#include <glad/gl.h>
#include <FastNoise/FastNoiseLite.h>
#include <iostream>

#include "Vertex.hpp"
#include "Shader.hpp"
#include "OrbitCamera.hpp"
#include "Light.hpp"

Terrain::Terrain()
    : shader("shaders/terrain.vert", "shaders/terrain.frag"), worldPos(-15.0f, -1.0f, -15.0f) {
    float cellSize = 0.2f;
    int gridSizeX = 3000;
    int gridSizeZ = 3000;

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(0.01f);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetFractalLacunarity(3.5f);
    noise.SetFractalGain(0.15f);

    for (int j = 0; j < gridSizeZ; j++) {
        for (int i = 0; i < gridSizeX; i++) {
            float height = 30;
            float y = noise.GetNoise(i * cellSize, j * cellSize) * height;
            glm::vec3 v = glm::vec3(i * cellSize, y, j * cellSize);

            float dx = 0.1f;
            glm::vec3 u1 =
                glm::vec3(i * cellSize + dx,
                          noise.GetNoise(i * cellSize + dx, j * cellSize) * height, j * cellSize);
            glm::vec3 u2 =
                glm::vec3(i * cellSize, noise.GetNoise(i * cellSize, j * cellSize + dx) * height,
                          j * cellSize + dx);
            glm::vec3 t1 = u1 - v;
            glm::vec3 t2 = u2 - v;
            glm::vec3 n = glm::normalize(glm::cross(t1, t2)) * -1.f;

            glm::vec3 col = glm::vec3(0.5f, 0.5f + (y / height + 1) / 4, 0.5f);
            if (y / height <= -0.5f) {
                col = glm::vec3(0.5f, 0.63f, 0.5f + (-y / height - 0.5f));
            }
            this->vertices.push_back({v, n, col});
        }
    }

    for (int j = 0; j < gridSizeZ - 1; j++) {
        for (int i = 0; i < gridSizeX - 1; i++) {
            int idx = j * gridSizeX + i;
            indices.push_back(idx);
            indices.push_back(idx + gridSizeX);
            indices.push_back(idx + 1);

            indices.push_back(idx + 1);
            indices.push_back(idx + gridSizeX);
            indices.push_back(idx + gridSizeX + 1);
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

    this->worldPos = glm::vec3(-gridSizeX * cellSize / 2, 0.0f, -gridSizeZ * cellSize / 2);

    std::cout << "TERRAIN: Num vertices: " << this->vertices.size() << std::endl;
}

Terrain::~Terrain() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
}

void Terrain::render(OrbitCamera &camera, Light &light) {
    this->shader.use();
    this->shader.setVec3("worldPos", this->worldPos);
    camera.applyToShader(this->shader);
    light.applyToShader(this->shader);

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
}
