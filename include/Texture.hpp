#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#define GL_SILENCE_DEPRECATION

#include <string>
#include <unordered_map>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

class Texture
{
private:
    GLuint m_TextureID;
    int m_Width;
    int m_Height;
    int m_NrChannels;

public:
    Texture(const std::string &path);
    ~Texture();

    void Bind(GLenum textureUnit = GL_TEXTURE0) const;
    void Unbind() const;

    GLuint GetTextureID() const { return m_TextureID; }
    std::pair<int, int> GetDimensions() const;

private:
    void LoadFromFile(const std::string &path);
    void SetDefaultParameters();
};

class TextureManager
{
private:
    std::unordered_map<std::string, Texture *> m_TextureCache;
    std::string m_TextureDefaultPath;

public:
    TextureManager();
    ~TextureManager();

    void AddTexture(const std::string &name, const std::string &path);
    Texture *GetTexture(const std::string &name) const;
    void RemoveTexture(const std::string &name);
    void ClearTextures();

    void inline SetTextureDefaultPath(const std::string &defaultPath) { m_TextureDefaultPath = defaultPath; }
};

#endif
