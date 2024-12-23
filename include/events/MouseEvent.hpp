#ifndef MOUSE_EVENT_HPP
#define MOUSE_EVENT_HPP

#include "Event.hpp"
#include <sstream>

// Base Mouse Event Class
// cannot instantiate directly
class MouseEvent : public Event
{
public:
    uint32_t GetCategoryFlags() const override
    {
        return static_cast<uint32_t>(EventCategory::Mouse);
    }

    float GetX() const { return m_X; }
    float GetY() const { return m_Y; }

protected:
    MouseEvent(float x, float y) : m_X(x), m_Y(y) {}
    float m_X, m_Y;
};

class MouseMovedEvent : public MouseEvent
{
public:
    MouseMovedEvent(float x, float y) : MouseEvent(x, y) {}

    static EventType GetStaticType() { return EventType::MouseMoved; }
    EventType GetType() const override { return GetStaticType(); }

    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "MouseMovedEvent: (" << m_X << ", " << m_Y << ")";
        return oss.str();
    }
};

// Mouse Button Event
// cannot instantiate directly
class MouseButtonEvent : public Event
{
public:
    uint32_t GetCategoryFlags() const override
    {
        return static_cast<uint32_t>(EventCategory::Mouse) | static_cast<uint32_t>(EventCategory::MouseButton);
    }

    int GetButton() const { return m_Button; }

protected:
    explicit MouseButtonEvent(int button) : m_Button(button) {}
    int m_Button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    explicit MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

    static EventType GetStaticType() { return EventType::MouseButtonPressed; }
    EventType GetType() const override { return GetStaticType(); }

    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "MouseButtonPressedEvent: " << m_Button;
        return oss.str();
    }
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    explicit MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

    static EventType GetStaticType() { return EventType::MouseButtonReleased; }
    EventType GetType() const override { return GetStaticType(); }

    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "MouseButtonReleasedEvent: " << m_Button;
        return oss.str();
    }
};

class MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(float xOffset, float yOffset)
        : m_XOffset(xOffset), m_YOffset(yOffset) {}

    static EventType GetStaticType() { return EventType::MouseScrolled; }
    EventType GetType() const override { return GetStaticType(); }

    uint32_t GetCategoryFlags() const override
    {
        return static_cast<uint32_t>(EventCategory::Mouse);
    }

    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "MouseScrolledEvent: (" << m_XOffset << ", " << m_YOffset << ")";
        return oss.str();
    }

    float GetXOffset() const { return m_XOffset; }
    float GetYOffset() const { return m_YOffset; }

private:
    float m_XOffset, m_YOffset;
};

#endif
