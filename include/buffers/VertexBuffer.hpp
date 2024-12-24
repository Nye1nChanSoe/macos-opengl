#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include "BufferLayout.hpp"
#include <memory>
#include <vector>

class VertexBuffer
{
public:
    VertexBuffer(const std::vector<float> &vecBufferData);
    ~VertexBuffer();

    void Bind() const;
    void UnBind() const;

    void SetLayout(const BufferLayout &layout) { m_Layout = layout; }
    BufferLayout &GetLayout() { return m_Layout; }

    static std::shared_ptr<VertexBuffer> Create(const std::vector<float> &vecBufferData);

private:
    uint32_t m_VertexBufferID;
    BufferLayout m_Layout; // to retrieve buffer layout from vertex buffer
};

#endif