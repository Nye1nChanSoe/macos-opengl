#ifndef LAYER_STACK_HPP
#define LAYER_STACK_HPP

#include "Layer.hpp"
#include <vector>
#include <algorithm>

class LayerStack
{
public:
    LayerStack() = default;
    ~LayerStack();

    void PushLayer(Layer *layer);
    void PopLayer(Layer *layer);

    void PushOverlay(Layer *overlay);
    void PopOverlay(Layer *overlay);

    std::vector<Layer *>::iterator begin() { return m_Layers.begin(); }
    std::vector<Layer *>::iterator end() { return m_Layers.end(); }

    std::vector<Layer *>::const_iterator begin() const { return m_Layers.begin(); }
    std::vector<Layer *>::const_iterator end() const { return m_Layers.end(); }

private:
    std::vector<Layer *> m_Layers;
    unsigned int m_LayerInsertIndex = 0;
};

#endif
