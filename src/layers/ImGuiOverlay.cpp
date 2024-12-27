#include "layers/ImGuiOverlay.hpp"
#include "Application.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "audio/OpenAL.hpp"
#include "layers/AudioLayer.hpp"

ImGuiOverlay::ImGuiOverlay()
    : Layer("ImGuiOverlay"), m_Time(0.0f)
{
}

ImGuiOverlay::~ImGuiOverlay() {}

void ImGuiOverlay::OnAttach()
{
    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard navigation
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable docking

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled, adjust ImGui style for multiple windows
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Regular.ttf", 13.0f);
    io.Fonts->Build();

    // Setup platform/renderer bindings
    GLFWwindow *window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410 core");
}

void ImGuiOverlay::OnDetach()
{
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiOverlay::OnUpdate(float deltaTime)
{
    // ImGui doesn't use update logic in most cases; skip implementation if unnecessary
    m_Time = deltaTime;
}

void ImGuiOverlay::OnRender()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ShowDockingSpace();
    ShowLayerManagementUI();
    ShowPerformanceMetrics();
    ShowGraphicsAndAudioInfo();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiOverlay::ShowLayerManagementUI()
{
    ImGui::Begin("Layer Management");

    auto &app = Application::Get();
    auto &layerStack = app.GetLayerStack();

    for (Layer *layer : layerStack)
    {
        bool isVisible = layer->IsVisible();
        if (ImGui::Checkbox(layer->GetName().c_str(), &isVisible))
        {
            layer->SetVisibility(isVisible);

            // Check if the layer is an AudioLayer
            if (auto *audioLayer = dynamic_cast<AudioLayer *>(layer))
            {
                auto audio = audioLayer->GetAudio();
                if (audio)
                {
                    if (isVisible)
                        audio->Play();
                    else
                        audio->Pause();
                }
            }
        }
    }
    ImGui::End();
}

void ImGuiOverlay::ShowPerformanceMetrics()
{
    float fps = 1.0f / m_Time;
    ImGui::Begin("Performance Metrics");
    ImGui::Text("FPS: %.2f", fps);
    ImGui::Text("Delta Time: %.3f ms", m_Time.GetMilliSeconds());
    ImGui::End();
}

void ImGuiOverlay::ShowGraphicsAndAudioInfo()
{
    // Retrieve grphics information
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    const GLubyte *vendor = glGetString(GL_VENDOR);
    const ALbyte *alVersion = alGetString(AL_VERSION);

    ImGui::Begin("Graphics and Audio Info");
    ImGui::Text("Renderer: %s", renderer);
    ImGui::Text("OpenGL Version: %s", version);
    ImGui::Text("OpenAL Version: %s", alVersion);
    ImGui::Text("Vendor: %s", vendor);
    ImGui::End();
}

void ImGuiOverlay::ShowDockingSpace()
{
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

        // Menu Bar for DockSpace
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                static bool showDemoWindow = false;
                ImGui::MenuItem("Show Demo Window", nullptr, &showDemoWindow);
                if (showDemoWindow)
                    ImGui::ShowDemoWindow();
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGuiID dockSpaceID = ImGui::GetID("DockSpace");
        ImGui::DockSpace(dockSpaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        ImGui::End();
    }
}