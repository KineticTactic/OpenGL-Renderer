#pragma once

#include <string>
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <chrono>
#include <functional>

class Shader {
  public:
    Shader(const char *vertexPath, const char *fragmentPath);
    Shader(const char *vertexPath, const char *tessControlPath, const char *tessEvalPath,
           const char *fragmentPath);
    Shader(const char *computePath);
    ~Shader();

    void use();
    // void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setMat4(const std::string &name, glm::mat4 const &matrix);
    void setVec3(const std::string &name, glm::vec3 const &vector);
    void setVec4(const std::string &name, glm::vec4 const &vector);

    void reloadIfModified();
    void onReload(std::function<void()> callback);

    static void reloadAll();

  private:
    unsigned int programID;
    std::map<unsigned int, std::string> srcFiles;
    std::map<std::string, int> uniformLocations;
    std::time_t lastModified = 0;
    std::function<void()> reloadCallback;

    void buildShaderProgram();
    int getUniformLocation(const std::string &name);
    unsigned int compileShader(const char *path, unsigned int type);
    unsigned int linkProgram(std::vector<unsigned int> shaders);

    static std::string readFile(const char *filePath);

    inline static std::vector<Shader *> shaders = {};
};