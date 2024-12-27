#define STB_IMAGE_IMPLEMENTATION
#include "Texture.hpp"
#include "stb_image.h"
#include <iostream>
#include "Logger.hpp"

Texture::Texture(const std::string &path)
    : m_TextureID(0), m_Width(0), m_Height(0), m_NrChannels(0), m_TextureType(GL_TEXTURE_2D)
{
    LoadFromFile(path);
    SetDefaultParameters();
}

Texture::Texture(const std::vector<std::string> &cubeFaces)
    : m_TextureType(GL_TEXTURE_CUBE_MAP)
{
    LoadCubemap(cubeFaces);
    SetDefaultParameters();
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

    GLenum format = (m_NrChannels == 4) ? GL_RGBA : GL_RGB;
    glGenTextures(1, &m_TextureID); // Generate unique texture ID
    glBindTexture(m_TextureType, m_TextureID);
    glTexImage2D(m_TextureType, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(m_TextureType);

    stbi_image_free(data);
}

void Texture::LoadCubemap(const std::vector<std::string> &cubeFaces)
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < cubeFaces.size(); i++)
    {
        unsigned char *data = stbi_load(cubeFaces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            GLenum internalFormat = (nrChannels == 4) ? GL_SRGB_ALPHA : GL_SRGB;
            Logger::Debug("Loaded cubemap face: {} ({}x{}, {} channels)", cubeFaces[i], width, height, nrChannels);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cerr << "Failed to load cubemap texture: " << cubeFaces[i] << std::endl;
        }
    }
}

void Texture::Bind(GLenum textureUnit) const
{
    glActiveTexture(textureUnit);
    glBindTexture(m_TextureType, m_TextureID);
}

void Texture::Unbind() const
{
    glBindTexture(m_TextureType, 0);
}

void Texture::SetDefaultParameters()
{
    if (m_TextureType == GL_TEXTURE_2D)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else if (m_TextureType == GL_TEXTURE_CUBE_MAP)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
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
