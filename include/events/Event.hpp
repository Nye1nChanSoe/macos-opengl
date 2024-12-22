#ifndef EVENT_HPP
#define EVENT_HPP

#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>
#include <sstream>
#include <iostream>

class Event
{
public:
    enum Category : uint32_t
    {
        None = 0,
        Keyboard = 1 << 0,                     // 0001
        Mouse = 1 << 1,                        // 0010
        MouseButton = 1 << 2,                  // 0100
        Window = 1 << 3,                       // 1000
        Input = Keyboard | Mouse | MouseButton // Combination
    };

    // Event Types
    enum class Type
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

    virtual Type GetType() const = 0;
    virtual uint32_t GetCategoryFlags() const = 0;
    virtual std::string ToString() const = 0;

    // Check if the event belongs to a specific category
    bool IsInCategory(Category category) const
    {
        return GetCategoryFlags() & category;
    }

    bool IsHandled() const { return m_Handled; }
    void SetHandled(bool handled) { m_Handled = handled; }

    // Overload the << operator for easy logging
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
    using EventCallback = std::function<void(Event &)>;

    // Subscribe to a specific event type
    void Subscribe(Event::Type type, const EventCallback &callback)
    {
        m_EventCallbacks[type].push_back(callback);
    }

    // Dispatch an event to the appropriate handlers
    void Dispatch(Event &event)
    {
        auto it = m_EventCallbacks.find(event.GetType());
        if (it != m_EventCallbacks.end())
        {
            for (auto &callback : it->second)
            {
                callback(event);
                if (event.IsHandled())
                {
                    break; // Stop propagation if handled
                }
            }
        }
    }

private:
    std::unordered_map<Event::Type, std::vector<EventCallback>> m_EventCallbacks;
};

#endif
