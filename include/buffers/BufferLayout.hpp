#ifndef BUFFER_LAYOUT_HPP
#define BUFFER_LAYOUT_HPP

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <string>
#include <vector>
#include <stdexcept>
#include "Logger.hpp"

// i prefix = integer
// without prefix = float
enum class BufferAttributeType
{
    iVec = 0,
    iVec2,
    iVec3,
    iVec4,
    Vec,
    Vec2,
    Vec3,
    Vec4,
    iMat2,
    iMat3,
    iMat4,
    Mat2,
    Mat3,
    Mat4
};

using ShaderLayoutName = std::string;
using Normalized = bool;

/**
 * Represent an attribute in a row
 * Example:
 *      first 3 vertex positions (x, y, z in 3D space)
 *      second 3 normals or UV coords (how surface responds to lighting)
 *      thrid 2 texture coords (0, 0) lower left to (1, 1) upper right
 */
struct BufferElement
{
    BufferAttributeType m_Type;
    ShaderLayoutName m_ShaderLayoutName; // layout (location = index) in vec3 aPos;
    Normalized m_IsNormalized;           // are these attribute points normalized
    GLenum m_OpenGLType;
    GLint m_Count; // number of components (count) in each attribute

    BufferElement(BufferAttributeType attributeType, const ShaderLayoutName &name, Normalized isNormalized = false)
        : m_Type(attributeType),
          m_ShaderLayoutName(name),
          m_IsNormalized(isNormalized),
          m_OpenGLType(GetGLenumFromAttribType(attributeType)),
          m_Count(GetCountFromType(attributeType))
    {
    }

    ~BufferElement() {};

    static GLint GetCountFromType(BufferAttributeType type)
    {
        switch (type)
        {
        case BufferAttributeType::iVec:
            return 1;
        case BufferAttributeType::iVec2:
            return 2;
        case BufferAttributeType::iVec3:
            return 3;
        case BufferAttributeType::iVec4:
            return 4;
        case BufferAttributeType::Vec:
            return 1;
        case BufferAttributeType::Vec2:
            return 2;
        case BufferAttributeType::Vec3:
            return 3;
        case BufferAttributeType::Vec4:
            return 4;
        case BufferAttributeType::iMat2:
            return 2 * 2;
        case BufferAttributeType::iMat3:
            return 3 * 3;
        case BufferAttributeType::iMat4:
            return 4 * 4;
        case BufferAttributeType::Mat2:
            return 2 * 2;
        case BufferAttributeType::Mat3:
            return 3 * 3;
        case BufferAttributeType::Mat4:
            return 4 * 4;
        default:
            Logger::Critical("Unknown BufferAttributeType");
            throw std::runtime_error("Unknown BufferAttributeType");
        }
    };

    static GLint GetSizeFromAttribType(BufferAttributeType type)
    {
        GLenum glEnum = GetGLenumFromAttribType(type);
        switch (type)
        {
        case BufferAttributeType::iVec:
            return GetSizeFromGLenum(glEnum) * 1;
        case BufferAttributeType::iVec2:
            return GetSizeFromGLenum(glEnum) * 2;
        case BufferAttributeType::iVec3:
            return GetSizeFromGLenum(glEnum) * 3;
        case BufferAttributeType::iVec4:
            return GetSizeFromGLenum(glEnum) * 4;
        case BufferAttributeType::Vec:
            return GetSizeFromGLenum(glEnum) * 1;
        case BufferAttributeType::Vec2:
            return GetSizeFromGLenum(glEnum) * 2;
        case BufferAttributeType::Vec3:
            return GetSizeFromGLenum(glEnum) * 3;
        case BufferAttributeType::Vec4:
            return GetSizeFromGLenum(glEnum) * 4;
        case BufferAttributeType::iMat2:
            return GetSizeFromGLenum(glEnum) * 2 * 2;
        case BufferAttributeType::iMat3:
            return GetSizeFromGLenum(glEnum) * 3 * 3;
        case BufferAttributeType::iMat4:
            return GetSizeFromGLenum(glEnum) * 4 * 4;
        case BufferAttributeType::Mat2:
            return GetSizeFromGLenum(glEnum) * 2 * 2;
        case BufferAttributeType::Mat3:
            return GetSizeFromGLenum(glEnum) * 3 * 3;
        case BufferAttributeType::Mat4:
            return GetSizeFromGLenum(glEnum) * 4 * 4;
        default:
            Logger::Critical("Unknown BufferAttributeType");
            throw std::runtime_error("Unknown BufferAttributeType");
        }
    }

    static GLenum GetGLenumFromAttribType(BufferAttributeType type)
    {
        switch (type)
        {
        case BufferAttributeType::iVec:
        case BufferAttributeType::iVec2:
        case BufferAttributeType::iVec3:
        case BufferAttributeType::iVec4:
        case BufferAttributeType::iMat2:
        case BufferAttributeType::iMat3:
        case BufferAttributeType::iMat4:
            return GL_INT;
        case BufferAttributeType::Vec:
        case BufferAttributeType::Vec2:
        case BufferAttributeType::Vec3:
        case BufferAttributeType::Vec4:
        case BufferAttributeType::Mat2:
        case BufferAttributeType::Mat3:
        case BufferAttributeType::Mat4:
            return GL_FLOAT;
        default:
            Logger::Critical("Unknown BufferAttributeType");
            throw std::runtime_error("Unknown BufferAttributeType");
        }
    };

    static std::size_t GetSizeFromGLenum(GLenum glEnum)
    {
        switch (glEnum)
        {
        case GL_BYTE:
            return sizeof(char);
        case GL_UNSIGNED_BYTE:
            return sizeof(unsigned char);
        case GL_SHORT:
            return sizeof(short);
        case GL_UNSIGNED_SHORT:
            return sizeof(unsigned short);
        case GL_INT:
            return sizeof(int);
        case GL_UNSIGNED_INT:
            return sizeof(unsigned int);
        case GL_FLOAT:
            return sizeof(float);
        case GL_DOUBLE:
            return sizeof(double);
        default:
            Logger::Critical("Unknown GLenum");
            throw std::runtime_error("Unknown GLenum");
        }
    };
};

/**
 * Represent the whole vertex data
 * Stride and Offset are calculated here
 *      Stride => bytes offset between each BufferElement (whole row)
 *      Offset => offset between each attribute
 */
class BufferLayout
{
private:
    GLsizei m_Stride;
    std::vector<BufferElement> m_BufferElements;

public:
    BufferLayout()
        : m_Stride(0), m_BufferElements()
    {
    }

    BufferLayout(std::initializer_list<BufferElement> bufferElements)
        : m_Stride(0), m_BufferElements(bufferElements)
    {
        for (auto &bufferElement : bufferElements)
        {
            m_Stride += BufferElement::GetSizeFromAttribType(bufferElement.m_Type);
        }
        Logger::Debug("BufferLayout Stride: {}", m_Stride);
    }

    inline GLsizei GetStride() const { return m_Stride; }
    inline const std::vector<BufferElement> &GetBufferElements() const
    {
        return m_BufferElements;
    }

    // iterable class
    std::vector<BufferElement>::iterator begin() { return m_BufferElements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_BufferElements.end(); }
    std::vector<BufferElement>::const_iterator begin() const { return m_BufferElements.begin(); }
    std::vector<BufferElement>::const_iterator end() const { return m_BufferElements.end(); }
};

#endif