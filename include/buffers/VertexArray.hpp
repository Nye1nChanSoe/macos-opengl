#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include <memory>
#include <vector>
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void UnBind() const;

    void AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer);
    void SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer);
    void AddVertexBuffer(const VertexBuffer &vertexBuffer);
    void SetIndexBuffer(const IndexBuffer &indexBuffer);

    const std::vector<std::shared_ptr<VertexBuffer>> &GetVertexBufferRefs() const { return m_VertexBuffersRefs; }
    const std::shared_ptr<IndexBuffer> &GetIndexBufferRefs() const { return m_IndexBufferRef; }

    static std::shared_ptr<VertexArray> Create();

private:
    uint32_t m_VertexArrayID;
    std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffersRefs;
    std::shared_ptr<IndexBuffer> m_IndexBufferRef;
};

#endif
