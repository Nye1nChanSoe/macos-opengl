#include "layers/ExampleLayer.hpp"
#include "buffers/BufferLayout.hpp"

ExampleLayer::ExampleLayer()
    : Layer("ExampleLayer")
{
}

ExampleLayer::~ExampleLayer() {}

void ExampleLayer::OnAttach()
{
    BufferLayout layout = {
        {BufferAttributeType::Vec3, "aPos"},
        {BufferAttributeType::Vec3, "aNormal"},
        {BufferAttributeType::Vec2, "aTexCoords"},
    };
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::OnUpdate(float deltaTime)
{
}

void ExampleLayer::OnRender()
{
}
