// Window.cpp
#include "Window.hpp"
#include <iostream>

Window::Window(const Properties &props)
    : m_Properties(props), m_Window(nullptr)
{
    Initialize();
}

Window::~Window()
{
    Shutdown();
}

void Window::Initialize()
{
    glfwInitHint(GLFW_ANGLE_PLATFORM_TYPE, GLFW_ANGLE_PLATFORM_TYPE_METAL);
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(GLFWErrorCallback);

    // OpenGL context hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = glfwCreateWindow(m_Properties.width, m_Properties.height, m_Properties.title.c_str(), nullptr, nullptr);
    if (!m_Window)
    {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_Window);
    SetVSync(m_Properties.vsync);
}

void Window::OnUpdate()
{
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

void Window::SetVSync(bool enabled)
{
    glfwSwapInterval(enabled ? 1 : 0);
    m_Properties.vsync = enabled;
}

bool Window::IsVSync() const
{
    return m_Properties.vsync;
}

void Window::Shutdown()
{
    if (m_Window)
        glfwTerminate();
}

void Window::GLFWErrorCallback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}
