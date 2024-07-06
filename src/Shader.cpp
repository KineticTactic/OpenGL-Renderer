#include "Shader.hpp"

#include <glad/gl.h>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    // Load shader from file
    const char *vertexSource = readFile(vertexPath).c_str();
    const char *fragmentSource = readFile(fragmentPath).c_str();

    std::cout << vertexPath << std::endl;
    std::cout << vertexSource << std::endl;
    std::cout << fragmentPath << std::endl;
    std::cout << fragmentSource << std::endl;

    // Load the vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    } else {
        std::cout << "Vertex shader compiled successfully\n";
    }

    // Load the fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    } else {
        std::cout << "Fragment shader compiled successfully\n";
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
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    } else {
        std::cout << "Shader program linked successfully\n";
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

std::string Shader::readFile(const char *filePath) {
    std::ifstream fileStream(filePath, std::ios::in);
    if (!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath
                  << ". File does not exist." << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    fileStream.close();
    return buffer.str();
}