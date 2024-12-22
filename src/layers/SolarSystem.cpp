#include "layers/SolarSystem.hpp"
#include <iostream>

SolarSystem::SolarSystem()
    : Layer("SolarSystem")
{
    std::cout << GetName() << " Added" << std::endl;
}

SolarSystem::~SolarSystem()
{
    std::cout << GetName() << " Removed" << std::endl;
}

void SolarSystem::OnAttach()
{
    std::cout << GetName() << " Attached" << std::endl;
}

void SolarSystem::OnDetach()
{
    std::cout << GetName() << " Detached" << std::endl;
}

void SolarSystem::OnUpdate(float deltaTime)
{
    std::cout << GetName() << ": " << "updating.DeltaTime : " << deltaTime << "\n ";
}

void SolarSystem::OnEvent(Event &event)
{
    std::cout << GetName() << ": " << "received event: " << event << "\n";
}

void SolarSystem::OnRender()
{
    std::cout << GetName() << ": " << "rendering.\n";
}
