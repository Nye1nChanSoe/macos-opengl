#ifndef KEY_EVENT_HPP
#define KEY_EVENT_HPP

#include "Event.hpp"
#include <sstream>

// Base Key Event Class
// cannot instantiate directly
class KeyEvent : public Event
{
public:
    uint32_t GetCategoryFlags() const override
    {
        return static_cast<uint32_t>(EventCategory::Keyboard) | static_cast<uint32_t>(EventCategory::Input);
    }

    int GetKeyCode() const { return m_KeyCode; }

protected:
    explicit KeyEvent(int keyCode) : m_KeyCode(keyCode) {}
    int m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(int keyCode, bool isRepeat = false)
        : KeyEvent(keyCode), m_IsRepeat(isRepeat) {}

    static EventType GetStaticType() { return EventType::KeyPressed; }
    EventType GetType() const override { return GetStaticType(); }

    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "KeyPressedEvent: " << m_KeyCode;
        return oss.str();
    }

private:
    bool m_IsRepeat;
};

class KeyReleasedEvent : public KeyEvent
{
public:
    explicit KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}

    static EventType GetStaticType() { return EventType::KeyReleased; }
    EventType GetType() const override { return GetStaticType(); }

    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "KeyReleasedEvent: " << m_KeyCode;
        return oss.str();
    }
};

class KeyRepeatEvent : public KeyEvent
{
public:
    explicit KeyRepeatEvent(int keyCode) : KeyEvent(keyCode) {}

    static EventType GetStaticType() { return EventType::KeyRepeat; }
    EventType GetType() const override { return GetStaticType(); }

    std::string ToString() const override
    {
        std::ostringstream oss;
        oss << "KeyRepeatEvent: " << m_KeyCode;
        return oss.str();
    }
};

#endif
