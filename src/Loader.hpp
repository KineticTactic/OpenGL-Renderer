#pragma once

#include <vector>
#include <string>

class Loader {
  public:
    // TODO: make std::vector<Vertex>
    static std::vector<float> loadOBJ(const std::string &path);
};