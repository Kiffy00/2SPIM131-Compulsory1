#pragma once
#include <string>
#include <vector>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include <GLFW/glfw3.h>
#include "ImGui/imgui_impl_opengl3.h"
#include "PhysicsEngine.h"

class UIManager {
public:
    UIManager(GLFWwindow* window, PhysicsEngine& physicsEngine);
    ~UIManager();

    void beginFrame();
    void endFrame();
    void render();

    void setQuadtree(Quadtree& quadtree) { this->quadtree = &quadtree; };
private:
    PhysicsEngine& physicsEngine;
    Quadtree* quadtree; 
    GLFWwindow* window;

    // UI state
    int selectedBallIndex;
    std::vector<std::string> ballNames;
    float velocityInput[3];
    float windInput[3];

    void buildUI();
    void updateBallNames();
};
