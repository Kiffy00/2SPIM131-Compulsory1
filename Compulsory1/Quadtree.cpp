#include "Quadtree.h"

Quadtree::Quadtree(const glm::vec4& bounds, bool isLeaf) : bounds(bounds), isLeaf(isLeaf) {
    if (!isLeaf) {
        createQuadrants();
    }
}

void Quadtree::createQuadrants() {
    float subWidth = bounds.z / 2.0f;
    float subHeight = bounds.w / 2.0f;
    float x = bounds.x;
    float y = bounds.y;

    // Create four leaf quadrants
    nodes[0] = std::make_unique<Quadtree>(glm::vec4(x + subWidth, y, subWidth, subHeight), true); // Top-right
    nodes[1] = std::make_unique<Quadtree>(glm::vec4(x, y, subWidth, subHeight), true);            // Top-left
    nodes[2] = std::make_unique<Quadtree>(glm::vec4(x, y + subHeight, subWidth, subHeight), true); // Bottom-left
    nodes[3] = std::make_unique<Quadtree>(glm::vec4(x + subWidth, y + subHeight, subWidth, subHeight), true); // Bottom-right
}


void Quadtree::clear() {
    objects.clear();
    for (auto& node : nodes) {
        node->objects.clear();
    }
}

int Quadtree::getIndex(RollingBall* ball) {
    int index = -1;
    float verticalMidpoint = bounds.x + bounds.z / 2.0f;
    float horizontalMidpoint = bounds.y + bounds.w / 2.0f;

    // Ball can completely fit within one of these quadrants
    bool leftQuadrant = (ball->position.x + ball->radius < verticalMidpoint);
    bool rightQuadrant = (ball->position.x - ball->radius >= verticalMidpoint);

    bool topQuadrant = (ball->position.z + ball->radius < horizontalMidpoint);
    bool bottomQuadrant = (ball->position.z - ball->radius >= horizontalMidpoint);

    if (topQuadrant) {
        if (leftQuadrant) 
            index = 1; // Top-left 
        else if (rightQuadrant) 
            index = 0; // Top-right 
    }
    else if (bottomQuadrant) {
        if (leftQuadrant) 
            index = 2; // Bottom-left
        else if (rightQuadrant)
            index = 3; // Bottom-right
    }

    return index;
}



void Quadtree::insert(RollingBall* ball) {
    int index = getIndex(ball);

    if (index != -1) {
        nodes[index]->objects.push_back(ball);
    }
    else {
        objects.push_back(ball); // If the ball does not fit completely within a quadrant, keep it in the parent node
    }
}


void Quadtree::retrieve(std::vector<RollingBall*>& returnObjects, RollingBall* ball) {
    int index = getIndex(ball);
    if (index != -1) {
        // Retrieve balls of same quadrant
        returnObjects.insert(returnObjects.end(), nodes[index]->objects.begin(), nodes[index]->objects.end());
    }

    // Add objects from the parent node (objects that couldn't be placed in any quadrant)
    returnObjects.insert(returnObjects.end(), objects.begin(), objects.end());
}


void Quadtree::getDebugInfo(std::vector<std::string>& debugLines, int indentLevel) {
    std::string indent(indentLevel * 2, ' '); // Indentation for readability

    // Display node bounds and object count
    std::stringstream ss;
    ss << indent << "Node Bounds: [" << bounds.x << ", " << bounds.y << ", " << bounds.z << ", " << bounds.w << "], Objects: " << objects.size();
    debugLines.push_back(ss.str());

    // Display each of the four quadrants
    for (int i = 0; i < 4; ++i) {
        std::stringstream childInfo;
        childInfo << indent << "  Quadrant " << i << " -> Bounds: ["
            << nodes[i]->bounds.x << ", " << nodes[i]->bounds.y << ", "
            << nodes[i]->bounds.z << ", " << nodes[i]->bounds.w << "], Objects: " << nodes[i]->objects.size();
        debugLines.push_back(childInfo.str());
    }
}