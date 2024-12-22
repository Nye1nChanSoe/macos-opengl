// Window.hpp
#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <string>

class Window
{
public:
    struct Properties
    {
        std::string title;
        int width;
        int height;
        bool vsync;
    };

    Window(const Properties &props);
    ~Window();

    void Initialize();
    void OnUpdate();
    void SetVSync(bool enabled);
    bool IsVSync() const;
    void Shutdown();

    GLFWwindow *GetNativeWindow() const { return m_Window; }

private:
    static void GLFWErrorCallback(int error, const char *description);

    Properties m_Properties;
    GLFWwindow *m_Window;
};

#endif
