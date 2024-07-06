#pragma once

#include <string>

class Shader {
  public:
    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();

    void use();
    // void setBool(const std::string &name, bool value) const;
    // void setInt(const std::string &name, int value) const;
    // void setFloat(const std::string &name, float value) const;

  private:
    unsigned int programID;

    static std::string readFile(const char *filePath);
};