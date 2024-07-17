#pragma once

#include <string>
#include <vector>

class Shader;
class Camera;

class Skybox {
  private:
    unsigned int vao, vbo, textureID;

    static Shader *shader;
    static std::vector<float> vertexData;

  public:
    Skybox(const std::string path);
    ~Skybox();

    void render(Camera &camera);

    static void initShader();
};