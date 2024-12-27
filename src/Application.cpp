#include "Application.hpp"
#include "Window.hpp"
#include "layers/SolarSystem.hpp"
#include "layers/ImGuiOverlay.hpp"
#include "layers/ExampleLayer.hpp"
#include "layers/AudioLayer.hpp"
#include "Time.hpp"
#include "Logger.hpp"

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
    // Hint: current added audio as a layer instead of overlay
    // Hint: Later synchronize audio with events in the Solar System layer
    auto *layer1 = new AudioLayer();
    m_LayerStack.PushLayer(layer1);

    auto *layer2 = new SolarSystemLayer();
    m_LayerStack.PushLayer(layer2);

    auto *layer3 = new ExampleLayer();
    m_LayerStack.PushLayer(layer3);

    auto *overlay1 = new ImGuiOverlay();
    m_LayerStack.PushOverlay(overlay1);
}

Application::~Application()
{
    // Cleanup resources
    Logger::Debug("Application shutting down");
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

        // Logger::Info("FPS: {}", 1 / deltaTime);

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
    Logger::Debug("Width: {}", e.GetWidth());
    Logger::Debug("Height: {}", e.GetHeight());
    if (e.GetWidth() == 0 && e.GetHeight() == 0)
    {
        m_Minimized = true;
        return false; // let the event propagate
    }

    m_Minimized = false;
    return false;
}
