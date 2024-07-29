#include "GrassRenderer.hpp"

#include <iostream>
#include <SOIL2/SOIL2.h>
#include "Camera.hpp"

// positions and texcoords
std::vector<float> grassVertices = {
    -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, //
    1.0f, -1.0f, -1.0f, 1.0f, 0.0f, //
    1.0f, 1.0f, -1.0f, 1.0f, 1.0f,  //
    -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, //
    1.0f, 1.0f, -1.0f, 1.0f, 1.0f,  //
    -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  //
    //
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, //
    1.0f, -1.0f, 1.0f, 1.0f, 0.0f,   //
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f,    //
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, //
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f,    //
    -1.0f, 1.0f, -1.0f, 0.0f, 1.0f   //
}; //

GrassRenderer::GrassRenderer() : shader("./shaders/Grass.vert", "./shaders/Grass.frag") {
    this->grassTexID =
        SOIL_load_OGL_texture("./assets/textures/grass2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
                              SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
                                  SOIL_FLAG_COMPRESS_TO_DXT);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &modelVBO);
    glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
    glBufferData(GL_ARRAY_BUFFER, grassVertices.size() * sizeof(float), grassVertices.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1);
    glBindVertexArray(0);
}

GrassRenderer::~GrassRenderer() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &modelVBO);
}

void GrassRenderer::addGrass(glm::vec3 pos) {
    grassPositions.push_back(pos);
}

void GrassRenderer::render(Camera &camera) {
    return;
    glDisable(GL_CULL_FACE);

    // std::cout << "Grass: " << grassPositions.size() << std::endl;

    // for (auto &pos : grassPositions) {
    //     std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
    // }

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * grassPositions.size(), grassPositions.data(),
                 GL_DYNAMIC_DRAW);

    this->shader.use();
    camera.applyToShader(this->shader);
    this->shader.setMat4("model", glm::mat4(1.0f));
    this->shader.setInt("grassTex", 0);

    glBindVertexArray(this->vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grassTexID);

    glDrawArraysInstanced(GL_TRIANGLES, 0, grassVertices.size() / 5, this->grassPositions.size());

    glEnable(GL_CULL_FACE);
}