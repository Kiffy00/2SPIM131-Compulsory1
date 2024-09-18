#include "UIManager.h"

UIManager::UIManager(GLFWwindow* window, PhysicsEngine& physicsEngine)
    : window(window), physicsEngine(physicsEngine), selectedBallIndex(0) {
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = nullptr;  // Disable setting saving


    // Setup ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Initialize UI state with chained assignment!
    velocityInput[0] = velocityInput[1] = velocityInput[2] = 0.0f;
    windInput[0] = windInput[1] = windInput[2] = 0.0f;

    // Initialize ball names
    updateBallNames();
}

UIManager::~UIManager() {
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::beginFrame() {
    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIManager::endFrame() {
    // Render ImGui UI
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::render() {
    buildUI();
}

void UIManager::buildUI() {
    static bool firstFrame = true; // Set window size once
    if (firstFrame) {
        float windowWidth = 400;
        float windowHeight = 200;

        ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        firstFrame = false;
    }

    // Begin main window
    ImGui::Begin("Main Window");

    // Create a tab bar for the UI window
    if (ImGui::BeginTabBar("MainTabBar")) {

        // Ball Control Tab
        if (ImGui::BeginTabItem("Ball Control")) {
            // Ball control code remains the same
            ImGui::Text("Ball Control");

            // Get the list of balls from physicsEngine
            std::vector<RollingBall>& physicsBalls = physicsEngine.getBalls();

            // Update ball names if the number of balls has changed
            if (ballNames.size() != physicsBalls.size()) {
                updateBallNames();
            }

            // Combo box to select a ball
            const char* comboLabel = ballNames[selectedBallIndex].c_str();
            if (ImGui::BeginCombo("Select Ball", comboLabel)) {
                for (size_t i = 0; i < ballNames.size(); ++i) {
                    bool isSelected = (selectedBallIndex == i);
                    if (ImGui::Selectable(ballNames[i].c_str(), isSelected)) {
                        selectedBallIndex = i;
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            // Display and edit the selected ball's velocity
            RollingBall& selectedBall = physicsBalls[selectedBallIndex];

            ImGui::Text("Position: (%.2f, %.2f, %.2f)", selectedBall.position.x, selectedBall.position.y, selectedBall.position.z);
            ImGui::Text("Velocity: (%.2f, %.2f, %.2f)", selectedBall.velocity.x, selectedBall.velocity.y, selectedBall.velocity.z);

            ImGui::InputFloat3("Add Velocity", velocityInput);

            if (ImGui::Button("Apply Velocity")) {
                selectedBall.velocity += glm::vec3(velocityInput[0], velocityInput[1], velocityInput[2]);
            }

            ImGui::EndTabItem();
        }

        // Wind Control Tab
        if (ImGui::BeginTabItem("Wind Control")) {
            ImGui::Text("Wind Control");

            ImGui::SliderFloat("Wind X", &windInput[0], -10.0f, 10.0f);
            ImGui::SliderFloat("Wind Y", &windInput[1], -10.0f, 10.0f);
            ImGui::SliderFloat("Wind Z", &windInput[2], -10.0f, 10.0f);

            // Update automatically
            physicsEngine.setWind(glm::vec3(windInput[0], windInput[1], windInput[2]));

            ImGui::EndTabItem();
        }

        // Quadtree Debug Tab
        if (ImGui::BeginTabItem("Quadtree Debug")) {
            ImGui::Text("Quadtree Debug Information");

            if (quadtree) {
                std::vector<std::string> quadtreeDebugInfo;
                quadtree->getDebugInfo(quadtreeDebugInfo);

                // Display the debug information
                for (const auto& line : quadtreeDebugInfo) {
                    ImGui::TextUnformatted(line.c_str());
                }
            }
            else {
                ImGui::Text("Quadtree not available.");
            }

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End(); 
}

void UIManager::updateBallNames() {
    ballNames.clear();
    std::vector<RollingBall>& physicsBalls = physicsEngine.getBalls();
    for (size_t i = 0; i < physicsBalls.size(); ++i) {
        ballNames.push_back("Ball " + std::to_string(i));
    }
    // Avoid invalid ball
    if (selectedBallIndex >= ballNames.size()) {
        selectedBallIndex = 0;
    }
}
