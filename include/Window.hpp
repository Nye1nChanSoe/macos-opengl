// Window.hpp
#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <string>
#include "events/Event.hpp"

class Window
{
public:
    // type alias that can hold any callable objects
    // callable objects -> lambda, free function, bound memeber functions
    // that takes a single Event& parameter and returns void
    using EventCallbackFn = std::function<void(Event &)>;
    struct Properties
    {
        int width;
        int height;
        std::string title;
        bool vsync;

        EventCallbackFn eventCallback;
    };

    Window(const Properties &props);
    ~Window();

    void Initialize();
    void OnUpdate();
    void SetVSync(bool enabled);
    void Shutdown();

    GLFWwindow *GetNativeWindow() const { return m_Window; }

    // this will be bound to Application's OnEvent method
    // props.eventCallback will point to Application::OnEvent
    void inline SetEventCallback(const EventCallbackFn &callback) { m_WindowProperties.eventCallback = callback; }

    // Cursor modes
    void DisableCursor();
    void EnableCursor();
    void HideCursor();

    bool inline IsVSync() const { return m_WindowProperties.vsync; }
    const int inline GetWidth() const { return m_WindowProperties.width; }
    const int inline GetHeight() const { return m_WindowProperties.height; }

private:
    static void GLFWErrorCallback(int error, const char *description);

    Properties m_WindowProperties;
    GLFWwindow *m_Window;
};

#endif
