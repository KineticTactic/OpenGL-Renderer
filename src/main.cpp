#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// #include <tinyobj/tiny_obj_loader.h>

#include "Context.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include "Light.hpp"
#include "Renderable.hpp"
#include "Loader.hpp"
#include "Terrain.hpp"

std::vector<float> cubeVertices = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};

std::vector<float> planeVertices = {-0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, //
                                    0.5f,  0.0f, -0.5f, 0.0f, 1.0f, 0.0f, //
                                    0.5f,  0.0f, 0.5f,  0.0f, 1.0f, 0.0f, //
                                    -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, //
                                    0.5f,  0.0f, 0.5f,  0.0f, 1.0f, 0.0f, //
                                    -0.5f, 0.0f, 0.5f,  0.0f, 1.0f, 0.0f};

int main() {
    GLFWwindow *window = Context::createWindow();
    Context::initOpenGL();

    Shader phongShader("shaders/Phong.vert", "shaders/Phong.frag");
    Shader flatShader("shaders/Flat.vert", "shaders/Flat.frag");
    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
    Light light(glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    Input::init(window, &camera);

    camera.move(glm::vec2(0.0f, 50.0f));

    Renderable cube(Loader::loadOBJ("./assets/monkey.obj"));
    Renderable lightCube(cubeVertices);
    lightCube.setScale(glm::vec3(0.2f));
    // Renderable plane(planeVertices);
    // plane.setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
    // plane.setScale(glm::vec3(10.0f));

    Terrain terrain;

    while (!glfwWindowShouldClose(window)) {
        // glClearColor(0.568f, 0.67f, 0.72f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Input::processInput();
        camera.move(glm::vec2(1.0f, 0.0f));

        glm::vec3 lightPos = glm::vec3(sin(glfwGetTime() * 2) * 4,
                                       3 * cos(glfwGetTime() * 1) * sin(glfwGetTime() * 1.5) + 3,
                                       cos(glfwGetTime() * 2) * 4);
        light.setPosition(lightPos);
        lightCube.setPosition(lightPos);

        phongShader.use();
        camera.applyToShader(phongShader);
        light.applyToShader(phongShader);
        phongShader.setVec3("objectColor", glm::vec3(0.87f, 0.43f, 0.98f));
        cube.render(phongShader);
        // phongShader.setVec3("objectColor", glm::vec3(0.2f, 0.5f, 0.7f));
        // plane.render(phongShader);

        flatShader.use();
        flatShader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
        camera.applyToShader(flatShader);
        lightCube.render(flatShader);

        terrain.render(camera, light);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}