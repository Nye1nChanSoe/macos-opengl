#ifndef WINDOW_EVENT_HPP
#define WINDOW_EVENT_HPP

#include "Event.hpp"
#include <sstream>

class WindowResizedEvent : public Event
{
public:
    WindowResizedEvent(int width, int height)
        : m_Width(width), m_Height(height) {}

    static EventType GetStaticType() { return EventType::WindowResized; }
    EventType GetType() const override { return GetStaticType(); }

    uint32_t GetCategoryFlags() const override
    {
        return static_cast<uint32_t>(EventCategory::Window);
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

class WindowClosedEvent : public Event
{
public:
    WindowClosedEvent() = default;

    static EventType GetStaticType() { return EventType::WindowClosed; }
    EventType GetType() const override { return GetStaticType(); }

    uint32_t GetCategoryFlags() const override
    {
        return static_cast<uint32_t>(EventCategory::Window);
    }

    std::string ToString() const override
    {
        return "WindowClosedEvent";
    }
};

#endif
