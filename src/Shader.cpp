#include "Shader.hpp"

#include <glad/gl.h>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    // Load shader from file
    std::string vertexSource = readFile(vertexPath);
    std::string fragmentSource = readFile(fragmentPath);

    const char *vertexSourceC = vertexSource.c_str();
    const char *fragmentSourceC = fragmentSource.c_str();

    // Load the vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSourceC, NULL);
    glCompileShader(vertexShader);

    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        std::cout << "[SHADER]: Vertex shader compiled successfully (" << vertexPath << ")\n";
    }

    // Load the fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSourceC, NULL);
    glCompileShader(fragmentShader);

    // Check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        std::cout << "[SHADER]: Fragment shader compiled successfully (" << fragmentPath << ")\n";
    }

    // Link the shaders
    this->programID = glCreateProgram();
    glAttachShader(this->programID, vertexShader);
    glAttachShader(this->programID, fragmentShader);
    glLinkProgram(this->programID);

    // Check for linking errors
    glGetProgramiv(this->programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->programID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        assert(false);
    } else {
        std::cout << "[SHADER]: Shader program linked successfully\n";
    }

    // Delete the shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(this->programID);
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

int Shader::getUniformLocation(const std::string &name) {
    if (this->uniformLocations.find(name) == this->uniformLocations.end()) {
        this->uniformLocations[name] = glGetUniformLocation(this->programID, name.c_str());
    }
    return this->uniformLocations[name];
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