#ifndef IMGUI_LAYER_HPP
#define IMGUI_LAYER_HPP

#include "Layer.hpp"

class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
    virtual ~ImGuiLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;

private:
    void ShowLayerManagementUI();
};

#endif // IMGUI_LAYER_HPP
