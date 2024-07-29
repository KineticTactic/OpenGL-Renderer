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
#include "QuadTree.hpp"

int main() {
    GLFWwindow *window = Context::createWindow();
    Context::initOpenGL();
    Debug::init();

    FlyCamera camera(glm::vec3(0.0f, 1.0f, -5.0f));
    Context::setCamera(window, camera);
    Input::init(window, &camera);

    Shader phongShader("shaders/Phong.vert", "shaders/Phong.frag");

    Skybox::initShader();
    Skybox skybox("./assets/skybox/compressed/");

    Light light(glm::vec3(1.0f, 1.0f, 1.0f));
    light.setDirection(glm::vec3(-1.0f, -1.0f, -1.0f));
    light.setIntensity(0.7f);

    Renderable monke(Loader::loadOBJ("./assets/monkey.obj"));
    monke.setScale(glm::vec3(1.0f));
    monke.setPosition(glm::vec3(0.0f, 1000.0f, 0.0f));

    Renderable plane(Primitives::planeVertices);
    plane.setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
    plane.setScale(glm::vec3(10.0f));

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

    // Shader grassShader("shaders/Grass.vert", "shaders/Grass.frag");
    // grassShader.use();

    double lastTime = glfwGetTime();
    double dt = 0.0;
    while (!glfwWindowShouldClose(window)) {
        Input::processInput(dt);
        terrain.update(camera);

        if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_E) == GLFW_PRESS) {
            monke.setPosition(camera.getPosition());
        }

        // DEPTH PASS
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        float near_plane = 0.1f, far_plane = 7500.f;
        glm::mat4 lightProjection =
            glm::ortho(-2000.f, 2000.f, -2000.f, 2000.f, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(
            glm::vec3(1.0f, 1.0f, 1.0f) * 2000.f + camera.getPosition(),
            glm::vec3(0.0f, 0.0f, 0.0f) + camera.getPosition(), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;
        terrain.renderDepthPass(lightSpaceMatrix);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_FRAMEBUFFER_SRGB);

        // RENDER PASS
        glViewport(0, 0, Context::getFramebufferSize().x, Context::getFramebufferSize().y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        phongShader.use();
        camera.applyToShader(phongShader);
        light.applyToShader(phongShader);
        phongShader.setVec3("objectColor", glm::vec3(0.87f, 0.43f, 0.98f));
        monke.render(phongShader);
        phongShader.setVec3("objectColor", glm::vec3(0.2f, 0.3f, 0.9f));
        plane.render(phongShader);

        terrain.render(camera, light, depthMap, lightSpaceMatrix);

        skybox.render(camera);

        glfwSwapBuffers(window);
        glfwPollEvents();

        double currentTime = glfwGetTime();
        dt = currentTime - lastTime;
        lastTime = currentTime;
        // std::cout << "Delta: " << dt * 1000 << std::endl;
    }

    glfwTerminate();

    return 0;
}