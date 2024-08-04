#include "pch.hpp"
#include "Skybox.hpp"

#include <SOIL2/SOIL2.h>
#include <GLAD/gl.h>

#include "Shader.hpp"
#include "Camera.hpp"

Shader *Skybox::shader = nullptr;

Skybox::Skybox(const std::string path) {
    this->textureID = SOIL_load_OGL_cubemap((path + "px.jpg").c_str(), (path + "nx.jpg").c_str(),
                                            (path + "py.jpg").c_str(), (path + "ny.jpg").c_str(),
                                            (path + "pz.jpg").c_str(), (path + "nz.jpg").c_str(),
                                            SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    if (0 == this->textureID) {
        std::cerr << "ERROR: SOIL loading error: \n" << SOIL_last_result() << std::endl;
    }

    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, Skybox::vertexData.size() * sizeof(float),
                 Skybox::vertexData.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
}

Skybox::~Skybox() {
    glDeleteTextures(1, &this->textureID);
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
}

void Skybox::render(Camera &camera) {
    Skybox::shader->use();
    Skybox::shader->setMat4("view", glm::mat4(glm::mat3(camera.getViewMatrix())));
    Skybox::shader->setMat4("projection", camera.getProjectionMatrix());
    Skybox::shader->setInt("skybox", 0);

    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(this->vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);
}

void Skybox::initShader() {
    Skybox::shader = new Shader("Skybox", "shaders/Skybox.vert", "shaders/Skybox.frag");
}

std::vector<float> Skybox::vertexData = {
    // positions
    -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
    -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

    1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};
