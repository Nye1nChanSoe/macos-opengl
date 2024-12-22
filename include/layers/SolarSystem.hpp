#include "Layer.hpp"

class SolarSystem : public Layer
{
public:
    SolarSystem();
    ~SolarSystem();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(float deltaTime) override;
    void OnEvent(Event &event) override;
    void OnRender() override;
};