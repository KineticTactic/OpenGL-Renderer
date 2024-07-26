#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <vector>

#include "Shader.hpp"

class Camera;

class GrassRenderer {
  private:
    Shader shader;

    GLuint vao, modelVBO, instanceVBO;
    GLuint grassTexID;

    std::vector<glm::vec3> grassPositions;

  public:
    GrassRenderer();
    ~GrassRenderer();

    void addGrass(glm::vec3 pos);
    void render(Camera &camera);
};