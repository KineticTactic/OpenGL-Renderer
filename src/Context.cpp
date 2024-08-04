#include "Context.hpp"

#include <iostream>

#include "Camera.hpp"
#include "Log.hpp"

GLFWwindow *Context::createWindow() {
    GLFWwindow *window;

    if (!glfwInit()) {
        LOG_CRITICAL("[CONTEXT]: Failed to initialize GLFW");
        return nullptr;
    }
    LOG_INFO("[CONTEXT]: Initialized GLFW");

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(1366, 768, "Hello World", NULL, NULL);
    if (!window) {
        LOG_ERROR("[CONTEXT]: Failed to create GLFW window\n");
        glfwTerminate();
        return nullptr;
    }
    LOG_INFO("[CONTEXT]: Created GLFW window");
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    // glfwSwapInterval(0);

    // hide cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return window;
}

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
                            GLsizei length, const char *message, const void *userParam) {
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        std::cout << "Source: API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        std::cout << "Source: Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        std::cout << "Source: Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        std::cout << "Source: Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        std::cout << "Source: Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        std::cout << "Source: Other";
        break;
    }
    std::cout << std::endl;

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "Type: Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "Type: Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "Type: Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "Type: Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "Type: Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        std::cout << "Type: Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        std::cout << "Type: Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        std::cout << "Type: Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "Type: Other";
        break;
    }
    std::cout << std::endl;

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "Severity: high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "Severity: medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "Severity: low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cout << "Severity: notification";
        break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void Context::initOpenGL() {
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        LOG_CRITICAL("Failed to initialize OpenGL context");
        return;
    }
    // std::cout << "Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "."
    //           << GLAD_VERSION_MINOR(version) << "\n";
    LOG_INFO("Loaded OpenGL {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    glViewport(0, 0, 1366, 768);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    glPrimitiveRestartIndex(0xFFFFFFFF);
    glPatchParameteri(GL_PATCH_VERTICES, 4);
}

void Context::setCamera(GLFWwindow *window, Camera &camera) {
    glfwSetWindowUserPointer(window, &camera);
}

glm::vec2 Context::getFramebufferSize() {
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    return glm::vec2(width, height);
}

void Context::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    if (width == 0 || height == 0)
        return;
    glViewport(0, 0, width, height);

    Camera *camera = (Camera *)glfwGetWindowUserPointer(window);
    camera->setAspectRatio((float)width / (float)height);
}
