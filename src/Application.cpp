#include "Application.hpp"
#include "Window.hpp"
#include <iostream>
#include <memory>
#include "layers/SolarSystem.hpp"
#include "layers/ImGuiLayer.hpp"

Application *Application::s_Instance = nullptr;

Application::Application()
{
    if (s_Instance)
    {
        throw std::runtime_error("Application already exists!");
    }
    s_Instance = this;

    Window::Properties props = {1000, 700, "macos-opengl", true};

    m_Window = std::make_unique<Window>(props);
    m_Window->SetEventCallback(
        [this](Event &event)
        { OnEvent(event); });

    auto *layer1 = new SolarSystem();
    m_LayerStack.PushLayer(layer1);

    auto *layer2 = new ImGuiLayer();
    m_LayerStack.PushOverlay(layer2);
}

Application::~Application()
{
    // Cleanup resources
    std::cout << "Application shutting down.\n";
}

void Application::OnEvent(Event &event)
{
    // Dispatch events to layers in reverse order (from topmost to bottommost)
    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
    {
        (*--it)->OnEvent(event);
        if (event.IsHandled())
        {
            break; // Stop propagation if the event is handled
        }
    }

    // Handle core application events
    if (event.GetType() == Event::Type::WindowClosed)
    {
        m_Running = false;
    }
}

void Application::Run()
{
    while (m_Running)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update all layers
        for (Layer *layer : m_LayerStack)
        {
            if (layer->IsVisible())
                layer->OnUpdate(0.016f); // Fixed delta time (60 FPS)
        }

        // Render all layers
        for (Layer *layer : m_LayerStack)
        {
            if (layer->IsVisible())
                layer->OnRender();
        }

        // swap buffers and poll events
        m_Window->OnUpdate();
    }
}
