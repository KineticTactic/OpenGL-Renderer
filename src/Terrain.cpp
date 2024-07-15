#include "Terrain.hpp"

#include <glad/gl.h>
#include <FastNoise/FastNoiseLite.h>

#include "Vertex.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Light.hpp"

Terrain::Terrain()
    : shader("shaders/terrain.vert", "shaders/terrain.frag"), worldPos(-15.0f, -1.0f, -15.0f) {
    float cellSize = 0.05f;
    int gridSizeX = 600;
    int gridSizeZ = 600;

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(0.2f);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetFractalLacunarity(3.5f);
    noise.SetFractalGain(0.2f);

    for (int j = 0; j < gridSizeZ; j++) {
        for (int i = 0; i < gridSizeX; i++) {
            float y1 = noise.GetNoise(i * cellSize, j * cellSize);
            float y2 = noise.GetNoise(i * cellSize, (j + 1) * cellSize);
            float y3 = noise.GetNoise((i + 1) * cellSize, j * cellSize);
            float y4 = noise.GetNoise((i + 1) * cellSize, (j + 1) * cellSize);
            glm::vec3 v1 = glm::vec3(i * cellSize, y1, j * cellSize);
            glm::vec3 v2 = glm::vec3(i * cellSize, y2, (j + 1) * cellSize);
            glm::vec3 v3 = glm::vec3((i + 1) * cellSize, y3, j * cellSize);
            glm::vec3 v4 = glm::vec3((i + 1) * cellSize, y4, (j + 1) * cellSize);

            glm::vec3 normal = glm::cross(v2 - v1, v3 - v1);

            glm::vec3 col1 = glm::vec3(0.5f, 0.5f + (y1 + 1) / 4, 0.5f);
            if (y1 <= -0.5f) {
                col1 = glm::vec3(0.5f, 0.63f, 0.5f + (-y1 - 0.5f));
            }

            this->vertices.push_back({v1, normal, col1});
            this->vertices.push_back({v2, normal, col1});
            this->vertices.push_back({v3, normal, col1});

            this->vertices.push_back({v3, normal, col1});
            this->vertices.push_back({v2, normal, col1});
            this->vertices.push_back({v4, normal, col1});
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
}

Terrain::~Terrain() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
}

void Terrain::render(Camera &camera, Light &light) {
    this->shader.use();
    this->shader.setVec3("worldPos", this->worldPos);
    camera.applyToShader(this->shader);
    light.applyToShader(this->shader);

    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
}
