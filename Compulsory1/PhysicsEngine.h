#pragma once

#include "RollingBall.h"
#include "Wall.h"
#include <vector>
#include "Quadtree.h"

class PhysicsEngine {
public:
    PhysicsEngine(const glm::vec4& worldBounds)
        : gravity(0.0f, -9.81f, 0.0f), wind(0.0f, 0.0f, 0.0f), quadtree(worldBounds, false) {}

    Quadtree& getQuadtree() { return quadtree; }

    void update(float deltaTime);

    void addBall(const RollingBall& ball) { balls.push_back(ball); }
    inline std::vector<RollingBall>& getBalls() { return balls; }

    void addWall(const Wall& wall) { walls.push_back(wall); }

    void setWind(const glm::vec3& newWind) { wind = newWind; }

private:
    Quadtree quadtree;
    std::vector<RollingBall> balls;
    std::vector<Wall> walls;
    glm::vec3 gravity;
    glm::vec3 wind;
};
