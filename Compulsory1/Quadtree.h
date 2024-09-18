#pragma once

#include <vector>
#include <memory>
#include <sstream>
#include "RollingBall.h"
#include <glm/glm.hpp>

class Quadtree {
public:
    Quadtree(const glm::vec4& bounds, bool isLeaf);
    void clear();
    void insert(RollingBall* ball);
    void retrieve(std::vector<RollingBall*>& returnObjects, RollingBall* ball);
    void getDebugInfo(std::vector<std::string>& debugLines, int indentLevel = 0);

private:
    glm::vec4 bounds; // x, y, width, height
    std::vector<RollingBall*> objects;
    std::unique_ptr<Quadtree> nodes[4]; // Fixed array of 4 quadrants
    bool isLeaf;

    int getIndex(RollingBall* ball);
    void createQuadrants();
};
