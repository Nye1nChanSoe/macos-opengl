#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Window.hpp"
#include "LayerStack.hpp"
#include "events/Event.hpp"

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

private:
    std::unique_ptr<Window> m_Window;
    LayerStack m_LayerStack;
    bool m_Running = true;
    static Application *s_Instance;
};

#endif