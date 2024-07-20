#include "Chunk.hpp"

#include <FastNoise/FastNoiseLite.h>
#include <glad/gl.h>
#include <iostream>
#include <GLFW/glfw3.h>

#include "Vertex.hpp"
#include "Shader.hpp"

float Chunk::chunkSize = 254.0f;
float Chunk::cellSize = 10.0f;
std::vector<float> LODs = {1.0f, 5.0f, 10.0f, 20.0f, 100.0f, 200.0f};

Chunk::Chunk(int chunkX, int chunkZ)
    : worldPos({chunkX * Chunk::chunkSize, 0.0f, chunkZ * Chunk::chunkSize}) {

    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Chunk::~Chunk() {
    glDeleteTextures(1, &this->textureID);
}

void Chunk::generateBuffers() {
    glGenVertexArrays(1, &Chunk::vao);
    glBindVertexArray(Chunk::vao);

    // Vertex layout: vec3 pos, vec2 tex
    float pix = 1.0f / 256;
    std::vector<float> vertexData = {
        0,         0, 0,         pix,     pix,     //
        0,         0, chunkSize, pix,     1 - pix, //
        chunkSize, 0, 0,         1 - pix, pix,     //
        chunkSize, 0, chunkSize, 1 - pix, 1 - pix  //
    };

    glGenBuffers(1, &Chunk::vbo);
    glBindBuffer(GL_ARRAY_BUFFER, Chunk::vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, // position
                          (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, // tex
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void Chunk::deleteBuffers() {
    glDeleteVertexArrays(1, &Chunk::vao);
    glDeleteBuffers(1, &Chunk::vbo);
}

void Chunk::generate() {
    double start = glfwGetTime();

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(0.01f);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetFractalLacunarity(3.0f);
    noise.SetFractalGain(0.15f);

    FastNoiseLite noise2;
    noise2.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise2.SetFrequency(0.0015f);
    noise2.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise2.SetFractalLacunarity(1.0f);

    float cellSize = Chunk::chunkSize / (heightMapRes - 2);

    for (int j = 0; j < heightMapRes; j++) {
        for (int i = 0; i < heightMapRes; i++) {
            float height = 15;
            float height2 = 100;
            float y =
                noise.GetNoise((i - 1) * cellSize + worldPos.x, (j - 1) * cellSize + worldPos.z) *
                    height +
                noise2.GetNoise(i * cellSize + worldPos.x, j * cellSize + worldPos.z) * height2;
            this->heightMap.push_back(y);
        }
    }

    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, heightMapRes, heightMapRes, 0, GL_RED, GL_FLOAT,
                 this->heightMap.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    this->generated = true;

    double time = glfwGetTime() - start;
    std::cout << "[CHUNK]: Chunk generated: " << time * 1000 << "ms" << std::endl;
}

void Chunk::render(class Shader &shader) {
    // if (!this->generated)
    //     return;

    shader.setVec3("worldPos", this->worldPos);
    shader.setInt("heightMap", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureID);

    glBindVertexArray(Chunk::vao);
    glDrawArrays(GL_PATCHES, 0, 4);
}
