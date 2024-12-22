#ifndef LAYER_HPP
#define LAYER_HPP

#include <string>
#include "events/Event.hpp"

class Layer
{
public:
    explicit Layer(const std::string &name = "Unnamed Layer")
        : m_DebugName(name) {}

    virtual ~Layer() = default;

    // initialization codes
    virtual void OnAttach() {}

    // cleanup resources
    virtual void OnDetach() {}

    virtual void OnUpdate(float deltaTime) {}
    virtual void OnRender() {}
    virtual void OnEvent(Event &event) {}

    const inline std::string &GetName() const { return m_DebugName; }

protected:
    std::string m_DebugName;
};

#endif
