#include "ImGuiHandler.hpp"
#include <GLFW/glfw3.h>
#include <chrono>

ImGuiHandler::ImGuiHandler(GLFWwindow *window) : m_Window(window) {}

ImGuiHandler::~ImGuiHandler() {}

void ImGuiHandler::Initialize()
{
    // Initialize ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable docking
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard navigation

    // Style setup
    ImGui::StyleColorsDark();

    // Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 410 core");
}

void ImGuiHandler::BeginFrame()
{
    // Start a new ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Docking space setup
    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                       ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                                       ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground |
                                       ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", nullptr, windowFlags);
        ImGui::PopStyleVar(3);

        ImGuiID dockSpaceID = ImGui::GetID("DockSpace");
        ImGui::DockSpace(dockSpaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        ImGui::End();
    }
}

void ImGuiHandler::RenderShaderSelector(const std::vector<std::string> &shaderNames, int &selectedShader)
{
    ImGui::Begin("Shader Selector", nullptr, ImGuiWindowFlags_NoCollapse);

    // Dropdown for shader selection
    if (ImGui::BeginCombo("Select Shader", shaderNames[selectedShader].c_str()))
    {
        for (int i = 0; i < shaderNames.size(); i++)
        {
            bool isSelected = (selectedShader == i);
            if (ImGui::Selectable(shaderNames[i].c_str(), isSelected))
            {
                selectedShader = i; // Update selected shader index
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::End();
}

void ImGuiHandler::RenderDebugWindow(float frameTime)
{
    ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_NoCollapse);

    // Display FPS
    ImGui::Text("FPS: %.1f", 1.0f / frameTime);
    ImGui::Text("Frame Time: %.3f ms", frameTime * 1000.0f);

    ImGui::End();
}

void ImGuiHandler::EndFrame()
{
    // Render ImGui frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiHandler::Cleanup()
{
    // Cleanup ImGui resources
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
