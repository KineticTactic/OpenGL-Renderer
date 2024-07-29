#include "Shader.hpp"

#include <glad/gl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    this->srcFiles[GL_VERTEX_SHADER] = std::string(vertexPath);
    this->srcFiles[GL_FRAGMENT_SHADER] = std::string(fragmentPath);
    this->buildShaderProgram();
    Shader::shaders.push_back(this);
}

Shader::Shader(const char *vertexPath, const char *tessControlPath, const char *tessEvalPath,
               const char *fragmentPath) {
    this->srcFiles[GL_VERTEX_SHADER] = std::string(vertexPath);
    this->srcFiles[GL_TESS_CONTROL_SHADER] = std::string(tessControlPath);
    this->srcFiles[GL_TESS_EVALUATION_SHADER] = std::string(tessEvalPath);
    this->srcFiles[GL_FRAGMENT_SHADER] = std::string(fragmentPath);
    this->buildShaderProgram();
    Shader::shaders.push_back(this);
}

Shader::Shader(const char *computePath) {
    this->srcFiles[GL_COMPUTE_SHADER] = std::string(computePath);
    this->buildShaderProgram();
    Shader::shaders.push_back(this);
}

Shader::~Shader() {
    glDeleteProgram(this->programID);
    // https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
    Shader::shaders.erase(std::remove(Shader::shaders.begin(), Shader::shaders.end(), this),
                          Shader::shaders.end());
}

void Shader::use() {
    glUseProgram(this->programID);
}

void Shader::setInt(const std::string &name, int value) {
    glUniform1i(this->getUniformLocation(name), value);
}

void Shader::setFloat(const std::string &name, float value) {
    glUniform1f(this->getUniformLocation(name), value);
}

void Shader::setMat4(const std::string &name, glm::mat4 const &matrix) {
    glUniformMatrix4fv(this->getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setVec3(const std::string &name, glm::vec3 const &vector) {
    glUniform3fv(this->getUniformLocation(name), 1, &vector[0]);
}

void Shader::setVec4(const std::string &name, glm::vec4 const &vector) {
    glUniform4fv(this->getUniformLocation(name), 1, &vector[0]);
}

void Shader::reloadIfModified() {
    for (auto it = this->srcFiles.begin(); it != this->srcFiles.end(); ++it) {
        unsigned int type = it->first;
        std::string path = it->second;

        std::filesystem::file_time_type lastModified = std::filesystem::last_write_time(path);
        const auto systemTime = std::chrono::clock_cast<std::chrono::system_clock>(lastModified);
        const auto time = std::chrono::system_clock::to_time_t(systemTime);

        if (time > this->lastModified) {
            this->lastModified = time;

            glDeleteProgram(this->programID);
            this->uniformLocations.clear();

            this->buildShaderProgram();
            std::cout << "[SHADER]: Shader reloaded (" << path << ")\n";

            if (this->reloadCallback != nullptr)
                this->reloadCallback();
            break;
        }
    }
}

void Shader::onReload(std::function<void()> callback) {
    this->reloadCallback = callback;
}

void Shader::reloadAll() {
    for (Shader *shader : Shader::shaders) {
        shader->reloadIfModified();
    }
}

int Shader::getUniformLocation(const std::string &name) {
    if (this->uniformLocations.find(name) == this->uniformLocations.end())
        this->uniformLocations[name] = glGetUniformLocation(this->programID, name.c_str());
    return this->uniformLocations[name];
}

void Shader::buildShaderProgram() {
    std::vector<unsigned int> shaders;

    for (auto it = this->srcFiles.begin(); it != this->srcFiles.end(); ++it) {
        unsigned int type = it->first;
        std::string path = it->second;
        unsigned int shader = compileShader(path.c_str(), type);
        shaders.push_back(shader);

        std::filesystem::file_time_type lastModified = std::filesystem::last_write_time(path);
        const auto systemTime = std::chrono::clock_cast<std::chrono::system_clock>(lastModified);
        const auto time = std::chrono::system_clock::to_time_t(systemTime);

        if (time > this->lastModified)
            this->lastModified = time;
    }

    this->programID = linkProgram(shaders);

    for (unsigned int shader : shaders)
        glDeleteShader(shader);
}

unsigned int Shader::compileShader(const char *path, unsigned int type) {
    std::string source = readFile(path);
    const char *sourceC = source.c_str();

    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &sourceC, NULL);
    glCompileShader(shader);

    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED: " << path << "\n" << infoLog << std::endl;
        assert(false);
    } else {
        std::cout << "[SHADER]: Shader compiled successfully (" << path << ")\n";
    }

    return shader;
}

unsigned int Shader::linkProgram(std::vector<unsigned int> shaders) {
    unsigned int program = glCreateProgram();
    for (unsigned int shader : shaders) {
        glAttachShader(program, shader);
    }
    glLinkProgram(program);

    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        assert(false);
    } else {
        std::cout << "[SHADER]: Shader program linked successfully\n";
    }

    return program;
}

std::string Shader::readFile(const char *filePath) {
    std::ifstream fileStream;
    fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fileStream.open(filePath);
    if (!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    fileStream.close();

    return buffer.str();
}