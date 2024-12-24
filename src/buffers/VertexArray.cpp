#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>
#include "buffers/VertexArray.hpp"

VertexArray::VertexArray()
    : m_VertexArrayID(0)
{
    glGenVertexArrays(1, &m_VertexArrayID);
    glBindVertexArray(m_VertexArrayID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_VertexArrayID);
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_VertexArrayID);
}

void VertexArray::UnBind() const
{
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer)
{
    if (!vertexBuffer->GetLayout().GetBufferElements().size())
    {
        Logger::Critical("Vertex buffer needs a layout");
        throw std::runtime_error("Vertex buffer needs a layout");
    }

    glBindVertexArray(m_VertexArrayID);
    vertexBuffer->Bind();

    auto &layout = vertexBuffer->GetLayout();
    int index = 0;
    std::size_t offset = 0;
    for (const auto &element : layout)
    {
        Logger::Debug("Vertex Array Offset: {}", offset * BufferElement::GetSizeFromGLenum(element.m_OpenGLType));
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            element.m_Count,
            element.m_OpenGLType,
            element.m_IsNormalized,
            layout.GetStride(),
            (const void *)(offset * BufferElement::GetSizeFromGLenum(element.m_OpenGLType)));
        index++;
        offset += element.m_Count;
    }
    m_VertexBuffersRefs.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexbuffer)
{
    glBindVertexArray(m_VertexArrayID);
    indexbuffer->Bind();

    m_IndexBufferRef = indexbuffer;
}

void VertexArray::AddVertexBuffer(const VertexBuffer &vertexBuffer)
{
}

void VertexArray::SetIndexBuffer(const IndexBuffer &indexBuffer)
{
}

std::shared_ptr<VertexArray> VertexArray::Create()
{
    return std::make_shared<VertexArray>();
}