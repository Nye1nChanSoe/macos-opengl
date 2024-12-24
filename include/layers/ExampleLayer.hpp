#ifndef EXAMPLE_LAYER_HPP
#define EXAMPLE_LAYER_HPP

#include "Layer.hpp"

class ExampleLayer : public Layer
{
public:
    ExampleLayer();
    virtual ~ExampleLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
};

#endif
