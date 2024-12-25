#include "layers/ExampleLayer.hpp"
#include "buffers/BufferLayout.hpp"
#include <glm/glm.hpp>

ExampleLayer::ExampleLayer()
    : Layer("ExampleLayer")
{
}

ExampleLayer::~ExampleLayer()
{
}

void ExampleLayer::OnAttach()
{
    glEnable(GL_DEPTH_TEST);

    std::vector<float> vertices = {
        // Positions         // Texture Coords
        -0.5f, 0.0f, -0.5f, 0.0f, 0.0f, // Bottom-left
        0.5f, 0.0f, -0.5f, 1.0f, 0.0f,  // Bottom-right
        0.5f, 0.0f, 0.5f, 1.0f, 1.0f,   // Top-right
        -0.5f, 0.0f, 0.5f, 0.0f, 1.0f,  // Top-left
        0.0f, 1.0f, 0.0f, 0.5f, 0.5f    // Apex
    };

    std::vector<unsigned int> indices = {
        // Base (two triangles)
        0, 1, 2, // First triangle of the base
        2, 3, 0, // Second triangle of the base
        // Side faces
        0, 1, 4, // Front face
        1, 2, 4, // Right face
        2, 3, 4, // Back face
        3, 0, 4  // Left face
    };

    BufferLayout layout = {
        {BufferAttributeType::Vec3, "aPos"},
        // {BufferAttributeType::Vec3, "aNormal"},
        {BufferAttributeType::Vec2, "aTexCoords"},
    };

    m_VAO = VertexArray::Create();
    m_VBO = VertexBuffer::Create(vertices);
    m_EBO = IndexBuffer::Create(indices);
    m_VBO->SetLayout(layout);
    m_VAO->AddVertexBuffer(m_VBO);
    m_VAO->SetIndexBuffer(m_EBO);

    m_ShaderManager = ShaderManager::Create();
    m_ShaderManager->AddShader("pyramid", "example_vertex_shader.glsl", "example_frag_shader.glsl");

    m_TextureManager = TextureManager::Create();
    m_TextureManager->AddTexture("pyramid", "earth.jpg");

    m_Model = glm::mat4(1.0f);
    m_View = glm::lookAt(glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_Projection = glm::perspective(glm::radians(45.0f), (float)1000 / 740, 0.1f, 100.0f);
}

void ExampleLayer::OnDetach()
{
    m_VAO->UnBind();
    m_VBO->UnBind();
    m_EBO->UnBind();
}

void ExampleLayer::OnUpdate(float deltaTime)
{
}

void ExampleLayer::OnRender()
{
    auto *pyramidShader = m_ShaderManager->GetShader("pyramid");
    auto *pyramidTexture = m_TextureManager->GetTexture("pyramid");
    pyramidShader->UseProgram();

    // Add rotation to the model matrix
    static float rotationAngle = 0.0f;
    rotationAngle += 0.003f;
    m_Model = glm::rotate(glm::mat4(1.0f), rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around the Y-axis

    pyramidShader->UploadUniformMat4("uModel", m_Model);
    pyramidShader->UploadUniformMat4("uView", m_View);
    pyramidShader->UploadUniformMat4("uProjection", m_Projection);

    pyramidTexture->Bind(GL_TEXTURE0);
    pyramidShader->UploadUniform1i("texture1", 0); // Explicitly set sampler to use unit 0

    m_VAO->Bind();
    glDrawElements(GL_TRIANGLES, m_EBO->GetCount(), GL_UNSIGNED_INT, 0);
    m_VAO->UnBind();

    pyramidShader->UnBind();
}
