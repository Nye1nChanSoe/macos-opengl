#include "Application.hpp"
#include "Window.hpp"
#include <iostream>
#include <memory>
#include "layers/SolarSystem.hpp"
#include "layers/ImGuiLayer.hpp"
#include "Time.hpp"

Application *Application::s_Instance = nullptr;

Application::Application()
{
    if (s_Instance)
        throw std::runtime_error("Application already exists!");
    s_Instance = this;

    m_Window = std::make_unique<Window>();

    // bound to Application::OnEvent
    // In Window.cpp props.eventCallback(event) will call this method
    m_Window->SetEventCallback(
        [this](Event &event)
        {
            OnEvent(event);
        });

    // TODO: make layer handling more robust
    auto *layer1 = new SolarSystemLayer();
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
    // Event registration
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowClosedEvent>([this](WindowClosedEvent &e)
                                           { return OnWindowClose(e); });
    dispatcher.Dispatch<WindowResizedEvent>([this](WindowResizedEvent &e)
                                            { return OnWindowResize(e); });

    // Dispatch events to layers in reverse order (from topmost to bottommost)
    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
    {
        (*--it)->OnEvent(event);
        if (event.IsHandled())
            break; // Stop propagation if the event is handled
    }
}

void Application::Run()
{
    while (m_Running)
    {
        float time = static_cast<float>(glfwGetTime());
        Time deltaTime = time - m_LastFrameTime;
        m_LastFrameTime = time;

        // std::cout << "FPS: " << 1 / deltaTime << std::endl;

        // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!m_Minimized)
        {
            for (Layer *layer : m_LayerStack)
                if (layer->IsVisible())
                    layer->OnUpdate(deltaTime);
            for (Layer *layer : m_LayerStack)
                if (layer->IsVisible())
                    layer->OnRender();
        }

        m_Window->OnUpdate();

        if (glfwGetKey(m_Window.get()->GetNativeWindow(), GLFW_KEY_ESCAPE))
            m_Running = false;
    }
}

bool Application::OnWindowClose(WindowClosedEvent &e)
{
    m_Running = false;
    return true;
}

bool Application::OnWindowResize(WindowResizedEvent &e)
{
    std::cout << "Width: " << e.GetWidth() << "\n";
    std::cout << "Height: " << e.GetHeight() << "\n";
    if (e.GetWidth() == 0 && e.GetHeight() == 0)
    {
        m_Minimized = true;
        return false; // let the event propagate
    }

    m_Minimized = false;
    return false;
}
