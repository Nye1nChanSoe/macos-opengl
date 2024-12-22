#ifndef MOUSE_EVENT_HPP
#define MOUSE_EVENT_HPP

#include "Event.hpp"
#include <sstream>

// Base Mouse Event Class
class MouseEvent : public Event
{
public:
    float GetX() const { return m_X; }
    float GetY() const { return m_Y; }

protected:
    MouseEvent(float x, float y) : m_X(x), m_Y(y) {}

    float m_X, m_Y;
};

// Mouse Moved Event
class MouseMovedEvent : public MouseEvent
{
public:
    MouseMovedEvent(float x, float y) : MouseEvent(x, y) {}

    Type GetType() const override { return Type::MouseMoved; }

    uint32_t GetCategoryFlags() const override
    {
        return Category::Mouse;
    }

    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "MouseMovedEvent: (" << m_X << ", " << m_Y << ")";
        return oss.str();
    }
};

// Mouse Button Event
class MouseButtonEvent : public Event
{
public:
    int GetButton() const { return m_Button; }

protected:
    MouseButtonEvent(int button) : m_Button(button) {}

    int m_Button;
};

// Mouse Button Pressed Event
class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

    Type GetType() const override { return Type::MouseButtonPressed; }

    uint32_t GetCategoryFlags() const override
    {
        return Category::Mouse | Category::MouseButton;
    }

    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "MouseButtonPressedEvent: " << m_Button;
        return oss.str();
    }
};

// Mouse Button Released Event
class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

    Type GetType() const override { return Type::MouseButtonReleased; }

    uint32_t GetCategoryFlags() const override
    {
        return Category::Mouse | Category::MouseButton;
    }

    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "MouseButtonReleasedEvent: " << m_Button;
        return oss.str();
    }
};

#endif // MOUSE_EVENT_H
