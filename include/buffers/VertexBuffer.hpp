#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include "BufferLayout.hpp"
#include <memory>

class VertexBuffer
{
public:
    VertexBuffer(float *data, uint32_t size);
    ~VertexBuffer();

    void Bind() const;
    void UnBind() const;

    void SetLayout(const BufferLayout &layout) { m_Layout = layout; }
    BufferLayout &GetLayout() { return m_Layout; }

    static std::shared_ptr<VertexBuffer> Create(float *data, uint32_t size);

private:
    uint32_t m_VertexBufferID;
    BufferLayout m_Layout; // to retrieve buffer layout from vertex buffer
};

#endif