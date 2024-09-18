#include "SceneGenerator.h"
#include "PrimitiveGenerator.h"
#include <random>
#include <iostream>

SceneGenerator::SceneGenerator(PhysicsEngine& physicsEngine, float boxHalfSize, int numBalls)
    : physicsEngine(physicsEngine), boxHalfSize(boxHalfSize), numBalls(numBalls) {
    generateScene();
}

void SceneGenerator::generateScene() {
    addWalls();
    generateBalls();
}

void SceneGenerator::addWalls() {
    // Add walls to the physics engine
    physicsEngine.addWall(Wall(glm::vec3(1.0f, 0.0f, 0.0f), boxHalfSize));  // Left wall
    physicsEngine.addWall(Wall(glm::vec3(-1.0f, 0.0f, 0.0f), boxHalfSize)); // Right wall
    physicsEngine.addWall(Wall(glm::vec3(0.0f, 0.0f, 1.0f), boxHalfSize));  // Back wall
    physicsEngine.addWall(Wall(glm::vec3(0.0f, 0.0f, -1.0f), boxHalfSize)); // Front wall
    physicsEngine.addWall(Wall(glm::vec3(0.0f, 1.0f, 0.0f), 4.0f));         // Floor wall

    // Generate floor mesh
    Mesh3D floorMesh = PrimitiveGenerator::createPlane(boxHalfSize * 2, boxHalfSize * 2, glm::vec3(0.2f));
    floorObject = std::make_shared<WorldObject>(floorMesh, glm::vec3(0.0f, -4.0f, 0.0f));
}

void SceneGenerator::generateBalls() {
    // Constants for ball generation
    const float minRadius = 0.5f;
    const float maxRadius = 2.5f;

    // Random number generators
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> radiusDist(minRadius, maxRadius);
    std::uniform_real_distribution<float> massDist(1.0f, 5.0f);
    std::uniform_real_distribution<float> colorDist(0.0f, 1.0f);

    // Generate balls
    for (int i = 0; i < numBalls; ++i) {
        float radius = radiusDist(gen);
        float mass = massDist(gen);

        // Adjust position distributions based on radius
        std::uniform_real_distribution<float> positionDistX(-boxHalfSize + radius, boxHalfSize - radius);
        std::uniform_real_distribution<float> positionDistY(-4.0f + radius, boxHalfSize - radius);
        std::uniform_real_distribution<float> positionDistZ(-boxHalfSize + radius, boxHalfSize - radius);

        glm::vec3 position;
        bool validPosition = false;
        int maxAttempts = 100;
        int attempts = 0;
        do {
            if (attempts++ > maxAttempts) {
                std::cout << "Failed to place ball " << i << " without intersections." << std::endl;
                break;
            }
            position = glm::vec3(positionDistX(gen), positionDistY(gen), positionDistZ(gen));

            // Check for overlaps with existing balls
            validPosition = true;
            for (const auto& existingBall : physicsEngine.getBalls()) {
                float distance = glm::length(position - existingBall.position);
                if (distance < (radius + existingBall.radius)) {
                    validPosition = false;
                    break;
                }
            }
        } while (!validPosition);

        if (!validPosition) {
            continue; // Skip adding this ball if a valid position wasn't found
        }

        // Generate random color
        glm::vec3 color(colorDist(gen), colorDist(gen), colorDist(gen));
        Mesh3D sphereMesh = PrimitiveGenerator::createSphere(1.0f, 24, 24, color);

        // Create ball
        RollingBall ball(mass, radius, position, glm::vec3(0.0f));
        physicsEngine.addBall(ball);

        // Create WorldObject
        std::shared_ptr<WorldObject> sphereObject = std::make_shared<WorldObject>(sphereMesh, position);
        sphereObject->scale = glm::vec3(radius);
        sphereObjects.push_back(sphereObject);  
    }
}

const std::vector<std::shared_ptr<WorldObject>>& SceneGenerator::getSphereObjects() const {
    return sphereObjects;
}

std::shared_ptr<WorldObject> SceneGenerator::getFloorObject() const {
    return floorObject;
}
