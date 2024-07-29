#include "Terrain.hpp"

#include <glad/gl.h>
#include <FastNoise/FastNoiseLite.h>
#include <iostream>
#include <algorithm>
#include <SOIL2/SOIL2.h>

#include "Vertex.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Chunk.hpp"
#include "QuadTree.hpp"

struct IntCoords {
    int x;
    int y;
};

std::vector<IntCoords> Spiral(int X, int Y) {
    int x, y, dx, dy;
    x = y = dx = 0;
    dy = -1;
    int t = std::max(X, Y);
    int maxI = t * t;
    std::vector<IntCoords> coords;
    for (int i = 0; i < maxI; i++) {
        if ((-X / 2 <= x) && (x <= X / 2) && (-Y / 2 <= y) && (y <= Y / 2)) {
            coords.push_back({x, y});
        }
        if ((x == y) || ((x < 0) && (x == -y)) || ((x > 0) && (x == 1 - y))) {
            t = dx;
            dx = -dy;
            dy = t;
        }
        x += dx;
        y += dy;
    }
    return coords;
}

Terrain::Terrain()
    : shader("shaders/terrain.vert", "shaders/terrain.tesc", "shaders/terrain.tese",
             "shaders/terrain.frag"),
      terrainGenCompute("shaders/TerrainGen.comp"),
      terrainNormalCompute("shaders/TerrainNormal.comp"),
      depthShader("shaders/terrain.vert", "shaders/TerrainDepth.tesc", "shaders/TerrainDepth.tese",
                  "shaders/TerrainDepth.frag") {
    std::vector<IntCoords> coords = Spiral(1, 1);
    for (auto &coord : coords) {
        // this->chunks.push_back(new Chunk(coord.x, coord.y));
    }

    // TODO: Put into static method
    Chunk::generateBuffers();
    this->shader.use();
    this->shader.setInt("heightMapRes", Chunk::heightMapRes - 2);

    this->rockTex =
        SOIL_load_OGL_texture("./assets/textures/rock_2k.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
                              SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
                                  SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_TEXTURE_REPEATS);

    this->grassFieldTex = SOIL_load_OGL_texture(
        "./assets/textures/grassfield1.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
            SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_TEXTURE_REPEATS);
    // this->snowTex =
    //     SOIL_load_OGL_texture("./assets/textures/snow_4k.jpg", SOIL_LOAD_AUTO,
    //     SOIL_CREATE_NEW_ID,
    //                           SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
    //                               SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_TEXTURE_REPEATS);
}

Terrain::~Terrain() {
    for (auto &chunk : this->chunks) {
        delete chunk;
    }
    // TODO: Put into static method
    Chunk::deleteBuffers();
}

void Terrain::update(Camera &camera) {

    // Generate quad tree
    QuadTree *tree = new QuadTree(glm::vec2(0.f, 0.f), TERRAIN_SIZE, MIN_CHUNK_SIZE);
    tree->insert(glm::vec2(camera.getPosition().x, camera.getPosition().z));

    // Loop over new nodes
    for (auto &node : tree->nodes) {
        bool alreadyPresent = false;
        // Search for the new node in the existing nodes
        for (int i = this->nodes.size() - 1; i >= 0; i--) {
            if (this->nodes[i].worldPos != node.worldPos || this->nodes[i].size != node.size)
                continue;

            // We found the node
            alreadyPresent = true;

            // If the existing node is a leaf node and the new node is not,
            // it means that in the new tree this node has been newly subdivided
            // Then we want to remove the chunk associated with this node
            // as we will generate new chunks in its place for the new leaf nodes
            if (this->nodes[i].isLeafNode && !node.isLeafNode) {
                // Remove the chunk associated with this node
                for (int j = 0; j < this->chunks.size(); j++) {
                    if (this->chunks[j]->worldPos.x == node.worldPos.x &&
                        this->chunks[j]->worldPos.z == node.worldPos.y) {
                        delete this->chunks[j];
                        this->chunks.erase(this->chunks.begin() + j);
                        break;
                    }
                }
            }

            break;
        }

        // If its a new node that is not already present, add it
        if (!alreadyPresent) {
            this->nodes.push_back(node);

            // If the new node is a leaf node, generate a chunk for it
            if (node.isLeafNode) {
                this->chunks.push_back(
                    new Chunk(glm::vec3(node.worldPos.x, 0, node.worldPos.y), node.size));
            }
        }
    }

    // Chunk generation, spread out over frames
    int numPerFrame = 2;
    int generated = 0;

    for (auto &chunk : this->chunks) {
        if (!chunk->isGenerated()) {
            chunk->generate(this->terrainGenCompute, this->terrainNormalCompute, this->grass);
            generated++;
        }

        if (generated >= numPerFrame)
            break;
    }

    std::cout << "Nodes: " << this->nodes.size() << ", Chunks: " << this->chunks.size()
              << std::endl;
}

void Terrain::render(Camera &camera, Light &light, unsigned int depthMap,
                     glm::mat4 &lightSpaceMatrix) {
    this->shader.use();
    camera.applyToShader(this->shader);
    light.applyToShader(this->shader);
    shader.setMat4("view", camera.getViewMatrix());
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    shader.setInt("shadowMap", 1);
    shader.setInt("rockTex", 2);
    shader.setInt("grassFieldTex", 3);
    // shader.setInt("snowTex", 3);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, this->rockTex);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, this->grassFieldTex);

    // glActiveTexture(GL_TEXTURE3);
    // glBindTexture(GL_TEXTURE_2D, this->snowTex);

    for (auto &chunk : this->chunks) {
        chunk->render(this->shader);
    }

    this->grass.render(camera);
}

void Terrain::renderDepthPass(glm::mat4 &lightSpaceMatrix) {
    this->depthShader.use();
    this->depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    for (auto &chunk : this->chunks) {
        chunk->render(this->depthShader);
    }
}
