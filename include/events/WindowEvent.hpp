#ifndef WINDOW_EVENT_HPP
#define WINDOW_EVENT_HPP

#include "Event.hpp"
#include <sstream>

// Window Resized Event
class WindowResizedEvent : public Event
{
public:
    WindowResizedEvent(int width, int height)
        : m_Width(width), m_Height(height) {}

    Type GetType() const override { return Type::WindowResized; }

    uint32_t GetCategoryFlags() const override
    {
        return Category::Window;
    }

    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "WindowResizedEvent: " << m_Width << "x" << m_Height;
        return oss.str();
    }

    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }

private:
    int m_Width, m_Height;
};

// Window Closed Event
class WindowClosedEvent : public Event
{
public:
    WindowClosedEvent() {}

    Type GetType() const override { return Type::WindowClosed; }

    uint32_t GetCategoryFlags() const override
    {
        return Category::Window;
    }

    std::string ToString() const override
    {
        return "WindowClosedEvent";
    }
};

#endif // WINDOW_EVENT_H
