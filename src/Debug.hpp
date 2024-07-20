#pragma once

class Shader;

class Debug {
  public:
    static void init();
    static void drawTexture(unsigned int texture);

  private:
    inline static unsigned int quadVAO = 0, quadVBO = 0;
    inline static Shader *screenShader = nullptr;
};