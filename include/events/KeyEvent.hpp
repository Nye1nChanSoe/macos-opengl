#ifndef KEY_EVENT_HPP
#define KEY_EVENT_HPP

#include "Event.hpp"
#include <sstream>

// Base Key Event Class
class KeyEvent : public Event
{
public:
    int GetKeyCode() const { return m_KeyCode; }

protected:
    KeyEvent(int keyCode) : m_KeyCode(keyCode) {}

    int m_KeyCode;
};

// Key Pressed Event
class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(int keyCode, bool isRepeat = false)
        : KeyEvent(keyCode), m_IsRepeat(isRepeat) {}

    Type GetType() const override { return Type::KeyPressed; }

    uint32_t GetCategoryFlags() const override
    {
        return Category::Keyboard | Category::Input;
    }

    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "KeyPressedEvent: " << m_KeyCode << " (repeat: " << m_IsRepeat << ")";
        return oss.str();
    }

private:
    bool m_IsRepeat;
};

// Key Released Event
class KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}

    Type GetType() const override { return Type::KeyReleased; }

    uint32_t GetCategoryFlags() const override
    {
        return Category::Keyboard | Category::Input;
    }

    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "KeyReleasedEvent: " << m_KeyCode;
        return oss.str();
    }
};

#endif // KEY_EVENT_H
