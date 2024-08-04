#include "pch.hpp"
#include "Debug.hpp"

#include <glad/gl.h>
#include "Shader.hpp"

void Debug::init() {
    Debug::screenShader = new Shader("Screen", "shaders/Screen.vert", "shaders/Screen.frag");

    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f, 1.0f,  0.0f, // 0.0f, 1.0f, //
        -1.0f, -1.0f, 0.0f, // 0.0f, 0.0f, //
        1.0f,  -1.0f, 0.0f, // 1.0f, 0.0f, //
        1.0f,  -1.0f, 0.0f, // 1.0f, 0.0f, //
        1.0f,  1.0f,  0.0f, // 1.0f, 1.0f
        -1.0f, 1.0f,  0.0f, // 0.0f, 1.0f, //
    };

    glGenVertexArrays(1, &Debug::quadVAO);
    glGenBuffers(1, &Debug::quadVBO);

    glBindVertexArray(Debug::quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, Debug::quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
}

void Debug::drawTexture(unsigned int texture) {
    Debug::screenShader->use();
    glBindVertexArray(Debug::quadVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
