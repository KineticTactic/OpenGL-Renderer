#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL2/SOIL2.h>
#include <random>

#include "Context.hpp"
#include "Shader.hpp"
#include "OrbitCamera.hpp"
#include "FlyCamera.hpp"
#include "Input.hpp"
#include "Light.hpp"
#include "Renderable.hpp"
#include "Loader.hpp"
#include "Terrain.hpp"
#include "Skybox.hpp"
#include "Debug.hpp"

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

std::vector<float> planeVertices = {-0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f,  //
                                    -0.5f, 0.0f, 0.5f,  0.0f, 1.0f, 0.0f,  //
                                    0.5f,  0.0f, -0.5f, 0.0f, 1.0f, 0.0f,  //
                                    0.5f,  0.0f, -0.5f, 0.0f, 1.0f, 0.0f,  //
                                    -0.5f, 0.0f, 0.5f,  0.0f, 1.0f, 0.0f,  //
                                    0.5f,  0.0f, 0.5f,  0.0f, 1.0f, 0.0f}; //

// positions and texcoords
std::vector<float> grassVertices = {
    -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, //
    1.0f, -1.0f, -1.0f, 1.0f, 0.0f, //
    1.0f, 1.0f, -1.0f, 1.0f, 1.0f,  //
    -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, //
    1.0f, 1.0f, -1.0f, 1.0f, 1.0f,  //
    -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  //
    //
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, //
    1.0f, -1.0f, 1.0f, 1.0f, 0.0f,   //
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f,    //
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, //
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f,    //
    -1.0f, 1.0f, -1.0f, 0.0f, 1.0f   //
}; //

int main() {
    GLFWwindow *window = Context::createWindow();
    Context::initOpenGL();
    Debug::init();

    FlyCamera camera(glm::vec3(0.0f, 1.0f, -5.0f));

    Context::setCamera(window, camera);
    Input::init(window, &camera);

    Shader phongShader("shaders/Phong.vert", "shaders/Phong.frag");
    Shader flatShader("shaders/Flat.vert", "shaders/Flat.frag");

    Light light(glm::vec3(1.0f, 1.0f, 1.0f));
    // light.setPosition(glm::vec3(1.2f, 25.0f, 2.0f));
    light.setDirection(glm::vec3(-1.0f, -1.0f, -1.0f));
    light.setIntensity(0.7f);

    // camera.orbit(glm::vec2(0.0f, 50.0f));

    Renderable cube(Loader::loadOBJ("./assets/monkey.obj"));
    cube.setScale(glm::vec3(1.0f));
    cube.setPosition(glm::vec3(0.0f, 1000.0f, 0.0f));
    Renderable lightCube(cubeVertices);
    lightCube.setScale(glm::vec3(0.2f));
    Renderable plane(planeVertices);
    // plane.setPosition(glm::vec3(0.0f, 256.0f, 0.0f));
    plane.setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
    plane.setScale(glm::vec3(10.0f));

    Skybox::initShader();
    Skybox skybox("./assets/skybox/compressed/");

    Terrain terrain;

    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;

    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    unsigned int grassTexID =
        SOIL_load_OGL_texture("./assets/textures/grass2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
                              SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
                                  SOIL_FLAG_COMPRESS_TO_DXT);
    unsigned int grassVAO, grassVBO;
    glGenVertexArrays(1, &grassVAO);
    glBindVertexArray(grassVAO);
    glGenBuffers(1, &grassVBO);
    glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
    glBufferData(GL_ARRAY_BUFFER, grassVertices.size() * sizeof(float), grassVertices.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glm::vec3 translations[10000];
    // Generate random positions 10x10 area
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(-20.0f, 20.0f);
    for (unsigned int i = 0; i < 10000; i++) {
        glm::vec3 translation;
        translation.x = distribution(generator);
        translation.z = distribution(generator);
        translation.y = 0.0f;
        translations[i] = translation;
    }

    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 10000, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1);
    glBindVertexArray(0);

    Shader grassShader("shaders/Grass.vert", "shaders/Grass.frag");
    grassShader.use();

    double lastTime = glfwGetTime();
    double dt = 0.0;
    while (!glfwWindowShouldClose(window)) {
        // glClearColor(0.568f, 0.67f, 0.72f, 1.0f);

        Input::processInput(dt);
        terrain.update(camera);

        if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_E) == GLFW_PRESS) {
            cube.setPosition(camera.getPosition());
        }

        // camera.orbit(glm::vec2(1.0f, 0.0f));

        // glm::vec3 lightPos = glm::vec3(sin(glfwGetTime() * 2) * 4,
        //                                3 * cos(glfwGetTime() * 1) * sin(glfwGetTime() * 1.5)
        //                                + 3, cos(glfwGetTime() * 2) * 4);
        // light.setPosition(lightPos);
        // lightCube.setPosition(lightPos);
        // glm::vec3 lightDir = glm::vec3(sin(glfwGetTime() * 2), -1.0f, cos(glfwGetTime() *
        // 2)); light.setDirection(lightDir);

        // DEPTH PASS
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        // // glCullFace(GL_FRONT);

        float near_plane = 0.1f, far_plane = 7500.f;
        glm::mat4 lightProjection =
            glm::ortho(-2000.f, 2000.f, -2000.f, 2000.f, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(
            glm::vec3(1.0f, 1.0f, 1.0f) * 2000.f + camera.getPosition(),
            glm::vec3(0.0f, 0.0f, 0.0f) + camera.getPosition(), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;
        terrain.renderDepthPass(lightSpaceMatrix);

        // RenderScene();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // RENDER PASS
        // glCullFace(GL_BACK); // don't forget to reset original culling face
        glViewport(0, 0, Context::getFramebufferSize().x, Context::getFramebufferSize().y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        phongShader.use();
        camera.applyToShader(phongShader);
        light.applyToShader(phongShader);
        phongShader.setVec3("objectColor", glm::vec3(0.87f, 0.43f, 0.98f));
        cube.render(phongShader);
        phongShader.setVec3("objectColor", glm::vec3(0.2f, 0.3f, 0.9f));
        plane.render(phongShader);

        // flatShader.use();
        // flatShader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
        // camera.applyToShader(flatShader);
        // lightCube.render(flatShader);

        // terrain.render(camera, light, depthMap, lightSpaceMatrix);

        glDisable(GL_CULL_FACE);
        grassShader.use();
        camera.applyToShader(grassShader);
        grassShader.setMat4("model", glm::mat4(1.0f));
        glBindVertexArray(grassVAO);
        grassShader.setInt("grassTex", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassTexID);
        glDrawArraysInstanced(GL_TRIANGLES, 0, grassVertices.size() / 5, 10000);

        glEnable(GL_CULL_FACE);

        skybox.render(camera);

        glfwSwapBuffers(window);
        glfwPollEvents();

        double currentTime = glfwGetTime();
        dt = currentTime - lastTime;
        lastTime = currentTime;
        std::cout << "Delta: " << dt * 1000 << std::endl;
    }

    glfwTerminate();

    return 0;
}