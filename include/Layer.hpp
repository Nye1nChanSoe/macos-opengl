#ifndef LAYER_HPP
#define LAYER_HPP

#include <string>
#include "events/Event.hpp"

class Layer
{
public:
    explicit Layer(const std::string &name = "Unnamed Layer", bool isVisible = true)
        : m_DebugName(name), m_Visible(isVisible) {}

    virtual ~Layer() = default;

    // initialization codes
    virtual void OnAttach() {}

    // cleanup resources
    virtual void OnDetach() {}

    virtual void OnUpdate(float deltaTime) {}
    virtual void OnRender() {}
    virtual void OnEvent(Event &event) {}

    const inline std::string &GetName() const { return m_DebugName; }
    inline void SetVisibility(bool visible) { m_Visible = visible; }
    inline bool IsVisible() const { return m_Visible; }

protected:
    std::string m_DebugName;
    bool m_Visible; // visibility flag (for ImGUI layer management)
};

#endif
