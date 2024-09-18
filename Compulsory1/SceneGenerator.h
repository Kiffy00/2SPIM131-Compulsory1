#include "PhysicsEngine.h"
#include "Renderer.h"
#include "WorldObject.h"
#include <vector>
#include <memory>

class SceneGenerator {
public:
    SceneGenerator(PhysicsEngine& physicsEngine, float boxHalfSize, int numBalls);

    const std::vector<std::shared_ptr<WorldObject>>& getSphereObjects() const;
    std::shared_ptr<WorldObject> getFloorObject() const;

private:
    PhysicsEngine& physicsEngine;
    float boxHalfSize;
    int numBalls;

    std::vector<std::shared_ptr<WorldObject>> sphereObjects;
    std::shared_ptr<WorldObject> floorObject;

    void generateScene();
    void addWalls();
    void generateBalls();
};
