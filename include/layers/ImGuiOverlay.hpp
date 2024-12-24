#ifndef IMGUI_LAYER_HPP
#define IMGUI_LAYER_HPP

#include "Layer.hpp"

class ImGuiOverlay : public Layer
{
public:
    ImGuiOverlay();
    virtual ~ImGuiOverlay();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;

private:
    void ShowLayerManagementUI();
    void ShowDockingSpace();
};

#endif // IMGUI_LAYER_HPP