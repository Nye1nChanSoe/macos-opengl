#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include <memory>

class IndexBuffer
{
public:
    IndexBuffer(uint32_t *data, uint32_t indexCount);
    ~IndexBuffer();

    void Bind() const;
    void UnBind() const;

    inline uint32_t GetCount() const { return m_Count; }

    static std::shared_ptr<IndexBuffer> Create(uint32_t *data, uint32_t indexCount);

private:
    uint32_t m_IndexBufferID;
    uint32_t m_Count;
};

#endif