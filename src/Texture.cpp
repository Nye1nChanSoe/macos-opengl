#define STB_IMAGE_IMPLEMENTATION
#include "Texture.hpp"
#include "stb_image.h"
#include <iostream>
#include "Logger.hpp"

Texture::Texture(const std::string &path)
    : m_TextureID(0), m_Width(0), m_Height(0), m_NrChannels(0)
{
    LoadFromFile(path);
}

Texture::~Texture()
{
    if (m_TextureID)
        glDeleteTextures(1, &m_TextureID);
}

void Texture::LoadFromFile(const std::string &path)
{
    unsigned char *data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_NrChannels, 0);
    if (!data)
    {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return;
    }

    glGenTextures(1, &m_TextureID); // Generate unique texture ID
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    GLenum format = (m_NrChannels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    SetDefaultParameters();

    stbi_image_free(data);
}

void Texture::Bind(GLenum textureUnit) const
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetDefaultParameters()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

std::pair<int, int> Texture::GetDimensions() const
{
    return {m_Width, m_Height};
}

TextureManager::TextureManager()
    : m_TextureDefaultPath("assets/textures/")
{
}

TextureManager::~TextureManager()
{
    ClearTextures();
}

void TextureManager::AddTexture(const std::string &name, const std::string &path)
{
    if (m_Textures.find(name) == m_Textures.end())
    {
        std::string fullPath = m_TextureDefaultPath + path;
        m_Textures[name] = new Texture(fullPath);
        Logger::Info("Texture: '{}' added from path {}", name, fullPath);
    }
    else
    {
        std::cerr << "Texture with name \"" << name << "\" already exists.\n";
    }
    m_TextureNames.push_back(name);
}

Texture *TextureManager::GetTexture(const std::string &name) const
{
    auto it = m_Textures.find(name);
    if (it != m_Textures.end())
    {
        return it->second;
    }

    std::cerr << "Texture with name \"" << name << "\" not found.\n";
    return nullptr;
}

void TextureManager::RemoveTexture(const std::string &name)
{
    auto it = m_Textures.find(name);
    if (it == m_Textures.end())
    {
        throw std::runtime_error("Texture with name " + name + " not found.");
    }

    // Delete the shader and remove from the map
    delete it->second;
    m_Textures.erase(it);

    // Remove the shader name from the list
    auto nameIt = std::find(m_TextureNames.begin(), m_TextureNames.end(), name);
    if (nameIt != m_TextureNames.end())
    {
        m_TextureNames.erase(nameIt);
    }

    Logger::Info("Texture removed: {}", name);
}

void TextureManager::ClearTextures()
{
    for (auto &pair : m_Textures)
    {
        delete pair.second;
    }
    m_Textures.clear();
    m_TextureNames.clear();
}

std::unique_ptr<TextureManager> TextureManager::Create()
{
    return std::make_unique<TextureManager>();
}
