#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include <memory>
#include <vector>

class IndexBuffer
{
public:
    IndexBuffer(const std::vector<unsigned int> &vecIndexBufferData);
    ~IndexBuffer();

    void Bind() const;
    void UnBind() const;

    inline uint32_t GetCount() const { return m_Count; }

    static std::shared_ptr<IndexBuffer> Create(const std::vector<unsigned int> &vecIndexBufferData);

private:
    uint32_t m_IndexBufferID;
    uint32_t m_Count;
};

#endif