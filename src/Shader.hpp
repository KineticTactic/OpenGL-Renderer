#pragma once

#include <string>
#include <glm/glm.hpp>
#include <map>

class Shader {
  public:
    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();

    void use();
    // void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setMat4(const std::string &name, glm::mat4 const &matrix);
    void setVec3(const std::string &name, glm::vec3 const &vector);
    void setVec4(const std::string &name, glm::vec4 const &vector);

  private:
    unsigned int programID;
    std::map<std::string, int> uniformLocations;

    int getUniformLocation(const std::string &name);

    static std::string readFile(const char *filePath);
};