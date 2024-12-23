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

    std::cout << "Vendor:   " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version:  " << glGetString(GL_VERSION) << std::endl;

    // Set window properties on GLFW context to retrieve them in callbacks
    glfwSetWindowUserPointer(m_Window, &m_WindowProperties);

    // GLFW Callbacks
    // Window Close
    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window)
                               {
                                   Properties &props = *(Properties *)glfwGetWindowUserPointer(window);
                                   WindowClosedEvent event;
                                   props.eventCallback(event); // Calls Application::OnEvent
                               });

    // Window Resize
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height)
                              {
    Properties& props = *(Properties*)glfwGetWindowUserPointer(window);
    props.width = width;
    props.height = height;

    WindowResizedEvent event(width, height);
    props.eventCallback(event); });

    // Key Press and Release
    glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int keycode, int scancode, int action, int mods)
                       {
    Properties& props = *(Properties*)glfwGetWindowUserPointer(window);

    switch (action) {
        case GLFW_PRESS: {
            KeyPressedEvent event(keycode, false); // Not a repeat
            props.eventCallback(event);
            break;
        }
        case GLFW_RELEASE: {
            KeyReleasedEvent event(keycode);
            props.eventCallback(event);
            break;
        }
        case GLFW_REPEAT: {
            KeyPressedEvent event(keycode, true); // Repeat event
            props.eventCallback(event);
            break;
        }
    } });

    // Mouse Movement
    glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xpos, double ypos)
                             {
    Properties& props = *(Properties*)glfwGetWindowUserPointer(window);

    MouseMovedEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
    props.eventCallback(event); });

    // Mouse Button Press and Release
    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods)
                               {
    Properties& props = *(Properties*)glfwGetWindowUserPointer(window);

    switch (action) {
        case GLFW_PRESS: {
            MouseButtonPressedEvent event(button);
            props.eventCallback(event);
            break;
        }
        case GLFW_RELEASE: {
            MouseButtonReleasedEvent event(button);
            props.eventCallback(event);
            break;
        }
    } });

    // Mouse Scroll
    glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset, double yOffset)
                          {
    Properties& props = *(Properties*)glfwGetWindowUserPointer(window);

    MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
    props.eventCallback(event); });
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
