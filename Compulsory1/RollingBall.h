#pragma once
#include <glm/glm.hpp>

class RollingBall {
public:
    float mass;
    float radius;
    glm::vec3 position;
    glm::vec3 velocity;

    RollingBall(float m, float r, glm::vec3 p, glm::vec3 v)
        : mass(m), radius(r), position(p), velocity(v) {}
};
