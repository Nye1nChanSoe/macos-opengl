#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>
#include "buffers/IndexBuffer.hpp"

IndexBuffer::IndexBuffer(const std::vector<unsigned int> &vecIndexBufferData)
    : m_IndexBufferID(0), m_Count(vecIndexBufferData.size())
{
    glGenBuffers(1, &m_IndexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vecIndexBufferData.size() * sizeof(unsigned int), vecIndexBufferData.data(), GL_STATIC_DRAW);
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

std::shared_ptr<IndexBuffer> IndexBuffer::Create(const std::vector<unsigned int> &vecIndexBufferData)
{
    return std::make_shared<IndexBuffer>(vecIndexBufferData);
}