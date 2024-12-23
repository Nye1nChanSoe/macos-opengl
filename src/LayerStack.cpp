#include "LayerStack.hpp"
#include <iostream>

LayerStack::~LayerStack()
{
    for (Layer *layer : m_Layers)
    {
        layer->OnDetach();
        delete layer;
    }
}

void LayerStack::PushLayer(Layer *layer)
{
    m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
    m_LayerInsertIndex++;
    layer->OnAttach();
    std::cout << "Layer pushed: " << layer->GetName() << "\n";
}

void LayerStack::PushOverlay(Layer *overlay)
{
    m_Layers.emplace_back(overlay);
    overlay->OnAttach();
    std::cout << "Overlay pushed: " << overlay->GetName() << "\n";
}

void LayerStack::PopLayer(Layer *layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
    if (it != m_Layers.begin() + m_LayerInsertIndex)
    {
        layer->OnDetach();
        m_Layers.erase(it);
        m_LayerInsertIndex--;
        std::cout << "Layer popped: " << layer->GetName() << "\n";
    }
}

void LayerStack::PopOverlay(Layer *overlay)
{
    auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
    if (it != m_Layers.end())
    {
        overlay->OnDetach();
        m_Layers.erase(it);
        std::cout << "Overlay popped: " << overlay->GetName() << "\n";
    }
}

Layer *LayerStack::FindLayerByName(const std::string &name) const
{
    for (Layer *layer : m_Layers)
    {
        if (layer->GetName() == name)
        {
            return layer;
        }
    }
    return nullptr;
}