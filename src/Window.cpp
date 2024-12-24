// Window.cpp
#include "Window.hpp"

#include "events/KeyEvent.hpp"
#include "events/WindowEvent.hpp"
#include "events/MouseEvent.hpp"
#include "Logger.hpp"

Window::Window(const InitializeWindowProps &props)
    : m_Window(nullptr)
{
    Initialize(props);
}

Window::~Window()
{
    Shutdown();
}

void Window::Initialize(const InitializeWindowProps &props)
{
    // initialize window's private properties
    m_WindowProperties.m_Width = props.width;
    m_WindowProperties.m_Height = props.height;
    m_WindowProperties.m_Title = props.title;
    m_WindowProperties.m_Vsync = props.vsync;

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

    m_Window = glfwCreateWindow(m_WindowProperties.m_Width, m_WindowProperties.m_Height, m_WindowProperties.m_Title.c_str(), nullptr, nullptr);
    if (!m_Window)
    {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_Window);
    SetVSync(m_WindowProperties.m_Vsync);

    Logger::Info("Vendor: {}", std::string(reinterpret_cast<const char *>(glGetString(GL_VENDOR))));
    Logger::Info("Renderer: {}", std::string(reinterpret_cast<const char *>(glGetString(GL_RENDERER))));
    Logger::Info("Version: {}", std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))));

    // Set window properties on GLFW context to retrieve them in callbacks
    glfwSetWindowUserPointer(m_Window, &m_WindowProperties);

    // GLFW Callbacks
    // Window Close
    glfwSetWindowCloseCallback(
        m_Window, [](GLFWwindow *window)
        {
            WindowProperties &props = *(WindowProperties *)glfwGetWindowUserPointer(window);
            WindowClosedEvent event;
            // (hint: application's constructor will setEventCallback to its OnEvent method)
            props.m_EventCallback(event); // Calls Application::OnEvent
        });

    // Window Resize
    glfwSetWindowSizeCallback(
        m_Window,
        [](GLFWwindow *window, int width, int height)
        {
            WindowProperties &props = *(WindowProperties *)glfwGetWindowUserPointer(window);
            props.m_Width = width;
            props.m_Height = height;

            WindowResizedEvent event(width, height);
            props.m_EventCallback(event);
        });

    // Key Press and Release
    glfwSetKeyCallback(
        m_Window,
        [](GLFWwindow *window, int keycode, int scancode, int action, int mods)
        {
            WindowProperties &props = *(WindowProperties *)glfwGetWindowUserPointer(window);
            switch (action)
            {
            case GLFW_PRESS:
            {
                KeyPressedEvent event(keycode, false); // Not a repeat
                props.m_EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent event(keycode);
                props.m_EventCallback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressedEvent event(keycode, true); // Repeat event
                props.m_EventCallback(event);
                break;
            }
            }
        });

    // Mouse Movement
    glfwSetCursorPosCallback(
        m_Window,
        [](GLFWwindow *window, double xpos, double ypos)
        {
            WindowProperties &props = *(WindowProperties *)glfwGetWindowUserPointer(window);
            MouseMovedEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
            props.m_EventCallback(event);
        });

    // Mouse Button Press and Release
    glfwSetMouseButtonCallback(
        m_Window,
        [](GLFWwindow *window, int button, int action, int mods)
        {
            WindowProperties &props = *(WindowProperties *)glfwGetWindowUserPointer(window);
            switch (action)
            {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent event(button);
                props.m_EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event(button);
                props.m_EventCallback(event);
                break;
            }
            }
        });

    // Mouse Scroll
    glfwSetScrollCallback(
        m_Window,
        [](GLFWwindow *window, double xOffset, double yOffset)
        {
            WindowProperties &props = *(WindowProperties *)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
            props.m_EventCallback(event);
        });
}

void Window::OnUpdate()
{
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

void Window::SetVSync(bool sync)
{
    glfwSwapInterval(sync);
    m_WindowProperties.m_Vsync = sync;
}

void Window::Shutdown()
{
    if (m_Window)
        glfwDestroyWindow(m_Window);
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
