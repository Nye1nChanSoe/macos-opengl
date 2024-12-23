#include "layers/ImGuiLayer.hpp"
#include "Application.hpp"
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::OnAttach()
{
    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard navigation
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable multi-viewport / platform windows

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled, adjust ImGui style for multiple windows
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup platform/renderer bindings
    GLFWwindow *window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410 core");
}

void ImGuiLayer::OnDetach()
{
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::OnUpdate(float deltaTime)
{
    // ImGui doesn't use update logic in most cases; skip implementation if unnecessary
}

void ImGuiLayer::OnRender()
{
    // Start a new ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ShowLayerManagementUI();

    // Render ImGui frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Handle multi-viewport rendering
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backupCurrentContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupCurrentContext);
    }
}

void ImGuiLayer::ShowLayerManagementUI()
{
    ImGui::Begin("Layer Management");

    auto &app = Application::Get();
    auto &layerStack = app.GetLayerStack();

    for (Layer *layer : layerStack)
    {
        bool isVisible = layer->IsVisible();
        if (ImGui::Checkbox(layer->GetName().c_str(), &isVisible))
            layer->SetVisibility(isVisible);
    }

    ImGui::End();
}