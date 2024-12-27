#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#define GL_SILENCE_DEPRECATION

#include <string>
#include <unordered_map>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <vector>

class Texture
{
private:
    GLuint m_TextureID;
    int m_Width;
    int m_Height;
    int m_NrChannels;
    GLenum m_TextureType;

public:
    Texture(const std::string &path);
    Texture(const std::vector<std::string> &cubeFaces);
    ~Texture();

    void Bind(GLenum textureUnit = GL_TEXTURE0) const;
    void Unbind() const;

    GLuint GetTextureID() const { return m_TextureID; }
    std::pair<int, int> GetDimensions() const;

private:
    void LoadFromFile(const std::string &path);
    void LoadCubemap(const std::vector<std::string> &cubeFaces);
    void SetDefaultParameters();
};

class TextureManager
{
private:
    std::unordered_map<std::string, Texture *> m_Textures;
    std::string m_TextureDefaultPath;
    std::vector<std::string> m_TextureNames;

public:
    TextureManager();
    ~TextureManager();

    void AddTexture(const std::string &name, const std::string &path);
    Texture *GetTexture(const std::string &name) const;
    void RemoveTexture(const std::string &name);

    void inline SetTextureDefaultPath(const std::string &defaultPath) { m_TextureDefaultPath = defaultPath; }

    const inline std::vector<std::string> &GetTextureNames() const { return m_TextureNames; }

    static std::unique_ptr<TextureManager> Create();

private:
    void ClearTextures();
};

#endif
