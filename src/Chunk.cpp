#include "Chunk.hpp"

#include <FastNoise/FastNoiseLite.h>
#include <glad/gl.h>
#include <iostream>
#include <GLFW/glfw3.h>

#include "Vertex.hpp"
#include "Shader.hpp"

Chunk::Chunk(int chunkX, int chunkZ)
    : worldPos({chunkX * Chunk::chunkSize, 0.0f, chunkZ * Chunk::chunkSize}) {

    glGenTextures(1, &this->heightMapID);
    glBindTexture(GL_TEXTURE_2D, this->heightMapID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, heightMapRes, heightMapRes, 0, GL_RED, GL_FLOAT,
                 nullptr);

    glGenTextures(1, &this->normalID);
    glBindTexture(GL_TEXTURE_2D, this->normalID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, heightMapRes, heightMapRes, 0, GL_RED, GL_FLOAT,
                 nullptr);
}

Chunk::~Chunk() {
    glDeleteTextures(1, &this->heightMapID);
}

// Vertex layout: vec3 pos, vec2 tex
// Credits: Cgpt
std::vector<float> generateSplitQuads(float chunkSize, int numSubdivisions, float pix) {
    std::vector<float> vertexData;

    float step = chunkSize / numSubdivisions;           // Size of each smaller quad
    float texStep = (1.0f - 2 * pix) / numSubdivisions; // Texture coordinate step size

    for (int i = 0; i < numSubdivisions; ++i) {
        for (int j = 0; j < numSubdivisions; ++j) {
            float x0 = i * step;
            float x1 = (i + 1) * step;
            float z0 = j * step;
            float z1 = (j + 1) * step;

            float s0 = pix + i * texStep;
            float s1 = pix + (i + 1) * texStep;
            float t0 = pix + j * texStep;
            float t1 = pix + (j + 1) * texStep;

            // Add vertices for the current quad
            vertexData.insert(vertexData.end(), {
                                                    x0, 0, z0, s0, t0, // Bottom-left
                                                    x0, 0, z1, s0, t1, // Top-left
                                                    x1, 0, z0, s1, t0, // Bottom-right
                                                    x1, 0, z1, s1, t1  // Top-right
                                                });
        }
    }

    return vertexData;
}

void Chunk::generateBuffers() {
    glGenVertexArrays(1, &Chunk::vao);
    glBindVertexArray(Chunk::vao);

    // Vertex layout: vec3 pos, vec2 tex
    float pix = 1.0f / heightMapRes;

    std::vector<float> vertexData = generateSplitQuads(Chunk::chunkSize, Chunk::subdivisions, pix);

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

void Chunk::generate(Shader &compute) {
    double start = glfwGetTime();

    glBindImageTexture(0, this->heightMapID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);

    compute.use();
    compute.setVec3("worldPos", this->worldPos);
    compute.setInt("heightMapRes", heightMapRes);
    glDispatchCompute(heightMapRes, heightMapRes, 1);
    // glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    // std::vector<float> data(heightMapRes * heightMapRes);
    // glBindTexture(GL_TEXTURE_2D, this->heightMapID); // Make sure to bind the texture before
    // glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, data.data());

    // //// Debug: print out some values to check if the texture was updated
    // for (int i = 0; i < 10; ++i) {
    //     std::cout << data[i] << std::endl;
    // }

    this->generated = true;

    double time = glfwGetTime() - start;
    std::cout << "[CHUNK]: Chunk generated: " << time * 1000 << "ms" << std::endl;
}

void Chunk::render(Shader &shader) {
    if (!this->generated)
        return;

    shader.setVec3("worldPos", this->worldPos);
    shader.setInt("heightMap", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->heightMapID);

    glBindVertexArray(Chunk::vao);
    glDrawArrays(GL_PATCHES, 0, subdivisions * subdivisions * 4);
}
