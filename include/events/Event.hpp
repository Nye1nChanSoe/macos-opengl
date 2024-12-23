#ifndef EVENT_HPP
#define EVENT_HPP

#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

// Base Event Class
class Event
{
public:
    enum class EventCategory : uint32_t
    {
        None = 0,
        Keyboard = 1 << 0,                     // 0001
        Mouse = 1 << 1,                        // 0010
        MouseButton = 1 << 2,                  // 0100
        Window = 1 << 3,                       // 1000
        Input = Keyboard | Mouse | MouseButton // Combination
    };

    enum class EventType
    {
        None = 0,
        KeyPressed,
        KeyReleased,
        KeyRepeat,
        KeyTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled,
        WindowResized,
        WindowClosed
    };

    virtual ~Event() = default;

    virtual EventType GetType() const = 0;
    virtual uint32_t GetCategoryFlags() const = 0;
    virtual std::string ToString() const = 0;

    bool IsInCategory(EventCategory category) const
    {
        return GetCategoryFlags() & static_cast<uint32_t>(category);
    }

    bool IsHandled() const { return m_Handled; }
    void SetHandled(bool handled) { m_Handled = handled; }

    friend std::ostream &operator<<(std::ostream &os, const Event &event)
    {
        os << event.ToString();
        return os;
    }

protected:
    bool m_Handled = false;
};

// Event Dispatcher
class EventDispatcher
{
public:
    explicit EventDispatcher(Event &event) : m_Event(event) {}

    // T = Type of the event to dispatch [WindowClosedEvent, KeyPressedEvent, etc..]
    // F = Type of the function or callable [Will be deduced by compiler]
    template <typename T, typename F>
    bool Dispatch(const F &func)
    {
        if (m_Event.GetType() == T::GetStaticType())
        {
            m_Event.SetHandled(func(static_cast<T &>(m_Event)));
            return true;
        }
        return false;
    }

private:
    Event &m_Event;
};

#endif