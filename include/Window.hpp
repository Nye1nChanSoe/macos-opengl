// Window.hpp
#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <string>
#include "events/Event.hpp"

struct InitializeWindowProps
{
    int height;
    int width;
    std::string title;
    bool vsync;

    InitializeWindowProps()
        : height(740), width(1000), title("macos-opengl"), vsync(true)
    {
    }
};

class Window
{
    // type alias that can hold any callable objects
    // callable objects -> lambda, free function, bound memeber functions
    // that takes a single Event& parameter and returns void
    using EventCallbackFn = std::function<void(Event &)>;

public:
    Window(const InitializeWindowProps &props = InitializeWindowProps());
    ~Window();

    void Initialize(const InitializeWindowProps &props);
    void OnUpdate();
    void SetVSync(bool sync);
    void Shutdown();

    GLFWwindow *GetNativeWindow() const { return m_Window; }

    // this will be bound to Application's OnEvent method
    // props.eventCallback will point to Application::OnEvent
    void inline SetEventCallback(const EventCallbackFn &callback) { m_WindowProperties.m_EventCallback = callback; }

    // Cursor modes
    void DisableCursor();
    void EnableCursor();
    void HideCursor();

    bool inline IsVSync() const { return m_WindowProperties.m_Vsync; }
    const int inline GetWidth() const { return m_WindowProperties.m_Width; }
    const int inline GetHeight() const { return m_WindowProperties.m_Height; }

private:
    struct WindowProperties
    {
        int m_Width;
        int m_Height;
        std::string m_Title;
        bool m_Vsync;
        EventCallbackFn m_EventCallback;
    };

    static void GLFWErrorCallback(int error, const char *description);

    WindowProperties m_WindowProperties;
    GLFWwindow *m_Window;
};

#endif
