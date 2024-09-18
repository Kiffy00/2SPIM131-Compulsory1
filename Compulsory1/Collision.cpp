#include "Collision.h"
#include <glm/glm.hpp>
#include <cmath>

static const float BOUNCE_FACTOR = 0.8f;
static const float EPSILON = 0.000001f;

float Collision::detectBallCollision(RollingBall& b1, RollingBall& b2) {
    auto A = b1.position - b2.position;
    auto B = b1.velocity - b2.velocity;
    float AB = glm::dot(A, B);
    float AA = glm::dot(A, A);
    float BB = glm::dot(B, B);
    float d = b1.radius + b2.radius;
    float rot = AB * AB - BB * (AA - d * d);

    float t = -1.0f;
    if (rot >= 0.0f) {
        t = -AB - std::sqrt(rot);
        if (BB > EPSILON) {
            t /= BB;
        }
    }
    return t;
}

void Collision::handleBallCollision(RollingBall& b1, RollingBall& b2) {
    glm::vec3 collisionNormal = glm::normalize(b1.position - b2.position);

    glm::vec3 relativeVelocity = b1.velocity - b2.velocity;
    float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);

    if (velocityAlongNormal > 0) {
        return;
    }

    float distance = glm::length(b1.position - b2.position);
    float penetrationDepth = (b1.radius + b2.radius) - distance;

    if (penetrationDepth > EPSILON) {
        glm::vec3 correction = collisionNormal * (penetrationDepth * 0.5f);
        b1.position += correction;
        b2.position -= correction;
    }

    float restitution = BOUNCE_FACTOR;

    float impulseScalar = -(1 + restitution) * velocityAlongNormal;
    impulseScalar /= (1 / b1.mass + 1 / b2.mass);

    glm::vec3 impulse = impulseScalar * collisionNormal;
    b1.velocity += impulse / b1.mass;
    b2.velocity -= impulse / b2.mass;
}

bool Collision::detectWallCollision(RollingBall& ball, const Wall& wall) {
    float distanceToWall = wall.signedDistance(ball.position);
    return distanceToWall <= ball.radius;
}

void Collision::handleWallCollision(RollingBall& ball, const Wall& wall) {
    glm::vec3 normal = wall.normal;
    float distanceToWall = wall.signedDistance(ball.position);

    float penetrationDepth = ball.radius - distanceToWall;
    if (penetrationDepth > 0.0f) {
        ball.position += normal * penetrationDepth;

        float velocityAlongNormal = glm::dot(ball.velocity, normal);
        if (velocityAlongNormal < 0) {
            float restitution = BOUNCE_FACTOR;
            ball.velocity -= (1 + restitution) * velocityAlongNormal * normal;

            glm::vec3 tangentVelocity = ball.velocity - glm::dot(ball.velocity, normal) * normal;
            float frictionCoefficient = 0.05f;
            ball.velocity -= frictionCoefficient * tangentVelocity;
        }
    }
}
