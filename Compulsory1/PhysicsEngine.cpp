#include "PhysicsEngine.h"
#include "Collision.h"
#include <glm/glm.hpp>

void PhysicsEngine::update(float deltaTime) {
    // Clear the quadtree
    quadtree.clear();

    // Insert all balls into the quadtree and apply forces
    for (auto& ball : balls) {
        ball.velocity += gravity * deltaTime;
        ball.velocity += wind * deltaTime;
        quadtree.insert(&ball);
    }

    // Ball-to-ball collisions
    for (auto& ball : balls) {
        std::vector<RollingBall*> possibleCollisions;
        quadtree.retrieve(possibleCollisions, &ball);

        for (auto& otherBall : possibleCollisions) {
            if (&ball == otherBall) continue;

            float distance = glm::length(ball.position - otherBall->position);
            float combinedRadii = ball.radius + otherBall->radius;

            if (distance <= combinedRadii) {
                Collision::handleBallCollision(ball, *otherBall);
            }
        }
    }

    // Ball-to-wall collisions
    for (auto& ball : balls) {
        for (const auto& wall : walls) {
            if (Collision::detectWallCollision(ball, wall)) {
                Collision::handleWallCollision(ball, wall);
            }
        }
    }

    // Update positions
    for (auto& ball : balls) {
        ball.position += ball.velocity * deltaTime;
    }
}