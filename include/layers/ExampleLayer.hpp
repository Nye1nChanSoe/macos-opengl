#ifndef EXAMPLE_LAYER_HPP
#define EXAMPLE_LAYER_HPP

#include "Layer.hpp"
#include "buffers/VertexBuffer.hpp"
#include "buffers/IndexBuffer.hpp"
#include "buffers/VertexArray.hpp"
#include "Shader.hpp"
#include "Texture.hpp";

class ExampleLayer : public Layer
{
public:
    ExampleLayer();
    virtual ~ExampleLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;

private:
    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<IndexBuffer> m_EBO;
    std::shared_ptr<VertexArray> m_VAO;
    std::unique_ptr<ShaderManager> m_ShaderManager;
    std::unique_ptr<TextureManager> m_TextureManager;

    glm::mat4 m_Model;
    glm::mat4 m_View;
    glm::mat4 m_Projection;
};

#endif
