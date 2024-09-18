#pragma once

#include "RollingBall.h"
#include "Wall.h"

class Collision {
public:
    static float detectBallCollision(RollingBall& b1, RollingBall& b2);
    static void handleBallCollision(RollingBall& b1, RollingBall& b2);

    static bool detectWallCollision(RollingBall& ball, const Wall& wall);
    static void handleWallCollision(RollingBall& ball, const Wall& wall);
};
