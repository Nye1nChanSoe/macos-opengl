// Window.cpp
#include "Window.hpp"
#include <iostream>

#include "events/KeyEvent.hpp"
#include "events/WindowEvent.hpp"
#include "events/MouseEvent.hpp"

Window::Window(const Properties &props)
    : m_WindowProperties(props), m_Window(nullptr)
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

    m_Window = glfwCreateWindow(m_WindowProperties.width, m_WindowProperties.height, m_WindowProperties.title.c_str(), nullptr, nullptr);
    if (!m_Window)
    {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_Window);
    SetVSync(m_WindowProperties.vsync);

    // Set Window User Pointer for Callbacks
    glfwSetWindowUserPointer(m_Window, &m_WindowProperties);

    // Logging OpenGL details
    std::cout << "Vendor:   " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version:  " << glGetString(GL_VERSION) << std::endl;

    // GLFW Callbacks
    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window)
                               {
        // is like taking the global pointer and casted it back stored in GLFW context
        Properties& props = *(Properties*)glfwGetWindowUserPointer(window);
        // The event that i implemented
        WindowClosedEvent event;
        // check the eventCallback is bound to Application::OnEvent method properly
        // Calls Application::OnEvent
        props.eventCallback(event); });

    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height)
                              {
        Properties& props = *(Properties*)glfwGetWindowUserPointer(window);
        props.width = width;
        props.height = height;

        WindowResizedEvent event(width, height);
        props.eventCallback(event); });

    glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int keycode, int scancode, int action, int mods)
                       {
        Properties& props = *(Properties*)glfwGetWindowUserPointer(window);

        switch (action)
        {
            case GLFW_PRESS:
                {
                    KeyPressedEvent event(keycode);
                    props.eventCallback(event);
                    break;
                }
            case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(keycode);
                    props.eventCallback(event);
                    break;
                }
        } });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xpos, double ypos)
                             {
        Properties& props = *(Properties*)glfwGetWindowUserPointer(window);

        MouseMovedEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
        props.eventCallback(event); });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods)
                               {
        Properties& props = *(Properties*)glfwGetWindowUserPointer(window);

        switch (action)
        {
            case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    props.eventCallback(event);
                    break;
                }
            case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    props.eventCallback(event);
                    break;
                }
        } });
}

void Window::OnUpdate()
{
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

void Window::SetVSync(bool enabled)
{
    glfwSwapInterval(enabled ? 1 : 0);
    m_WindowProperties.vsync = enabled;
}

void Window::Shutdown()
{
    if (m_Window)
        glfwTerminate();
}

void Window::DisableCursor()
{
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::EnableCursor()
{
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::HideCursor()
{
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Window::GLFWErrorCallback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}
