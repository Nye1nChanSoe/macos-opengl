#ifndef IMGUI_OVERLAY_HPP
#define IMGUI_OVERLAY_HPP

#include "Layer.hpp"
#include "Time.hpp"

class ImGuiOverlay : public Layer
{
private:
    Time m_Time;

public:
    ImGuiOverlay();
    virtual ~ImGuiOverlay();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;

private:
    void ShowLayerManagementUI();
    void ShowPerformanceMetrics();
    void ShowGraphicsInfo();
    void ShowDockingSpace();
};

#endif // IMGUI_LAYER_HPP
