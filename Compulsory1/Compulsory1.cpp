#include <iostream>

#include "Renderer.h"
#include "PrimitiveGenerator.h"
#include "RollingBall.h"
#include "PhysicsEngine.h"
#include "SceneGenerator.h"
#include "UIManager.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// GLFW window size
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char** argv) {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Compulsory 1", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    Renderer renderer;
    renderer.setAspect(SCR_WIDTH, SCR_HEIGHT);
    Camera camera(glm::vec3(0.0f, 1.0f, 10.0f));

    // Constants for ball generation
    const unsigned int NUM_BALLS = 20;
    const float minRadius = 0.5f;
    const float maxRadius = 2.5f;

    float boxHalfSize = 10.0f;

    // Create a physics engine
    glm::vec4 worldBounds(-boxHalfSize, -boxHalfSize, boxHalfSize * 2, boxHalfSize * 2); // x, y, width, height
    PhysicsEngine physicsEngine(worldBounds);

    // Generate scene for physics engine
    SceneGenerator sceneGenerator(physicsEngine, boxHalfSize, NUM_BALLS);

    // Get the sphere objects and floor object
    const auto& sphereObjects = sceneGenerator.getSphereObjects();
    auto floorObject = sceneGenerator.getFloorObject();

    // Initialize UI Manager
    UIManager uiManager(window, physicsEngine);
    uiManager.setQuadtree(physicsEngine.getQuadtree());

    // Initialize frame time tracking
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        // Process keyboard input and update deltaTime
        camera.processInput(window, deltaTime, lastFrame);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start ImGui frame
        uiManager.beginFrame();
        uiManager.render();

        // Update physics
        physicsEngine.update(deltaTime);

        // Update visual world objects with positions from the physics engine
        std::vector<RollingBall>& physicsBalls = physicsEngine.getBalls();
        for (size_t i = 0; i < sphereObjects.size(); ++i) {
            sphereObjects[i]->position = physicsBalls[i].position;
        }

        // Render all objects
        for (const auto& sphereObject : sphereObjects) {
            renderer.render(sphereObject, camera);
        }
        renderer.render(floorObject, camera);

        // End ImGui frame and render
        uiManager.endFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}