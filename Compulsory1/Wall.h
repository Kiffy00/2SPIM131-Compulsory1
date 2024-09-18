#pragma once
#include <glm/glm.hpp>

class Wall {
public:
    glm::vec3 normal;  // Front orientation of wall towards origin
    float distance;    // Distance from origin along the normal vector

    Wall(const glm::vec3& normal, float distance)
        : normal(glm::normalize(normal)), distance(-distance) {}

    // Helper function to calculate the signed distance of a point from the wall
    float signedDistance(const glm::vec3& point) const {
        return glm::dot(normal, point) - distance;
    }
};
