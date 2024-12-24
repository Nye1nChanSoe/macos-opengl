#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>
#include "buffers/VertexBuffer.hpp"

VertexBuffer::VertexBuffer(float *data, uint32_t size)
    : m_VertexBufferID(0)
{
    glGenBuffers(1, &m_VertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_VertexBufferID);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
}

void VertexBuffer::UnBind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

std::shared_ptr<VertexBuffer> VertexBuffer::Create(float *data, uint32_t size)
{
    return std::make_shared<VertexBuffer>(data, size);
}