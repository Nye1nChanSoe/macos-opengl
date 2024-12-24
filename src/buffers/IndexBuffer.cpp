#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>
#include "buffers/IndexBuffer.hpp"

IndexBuffer::IndexBuffer(uint32_t *data, uint32_t indexCount)
    : m_IndexBufferID(0), m_Count(indexCount)
{
    glGenBuffers(1, &m_IndexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_IndexBufferID);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
}

void IndexBuffer::UnBind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t *data, uint32_t indexCount)
{
    return std::make_shared<IndexBuffer>(data, indexCount);
}