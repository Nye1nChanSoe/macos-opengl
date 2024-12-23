#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Window.hpp"
#include "LayerStack.hpp"
#include "events/Event.hpp"
#include "events/WindowEvent.hpp"

class Application
{
public:
    Application();
    virtual ~Application();

    void Run();
    void OnEvent(Event &event);

    // singleton
    static Application &Get() { return *s_Instance; }
    inline Window &GetWindow() { return *m_Window; }

    LayerStack &GetLayerStack() { return m_LayerStack; }

private:
    bool m_Running = true;
    bool m_Minimized = false;
    float m_LastFrameTime = 0.0f;

    std::unique_ptr<Window> m_Window;

    LayerStack m_LayerStack;
    static Application *s_Instance;

private:
    // Eventhandlers
    bool OnWindowClose(WindowClosedEvent &e);
    bool OnWindowResize(WindowResizedEvent &e);
};

#endif
