#pragma once

#define GL_SILENCE_DEPRECATION

#include <string>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

class Texture
{
public:
    Texture(const std::string &path);
    ~Texture();

    void Bind(GLenum textureUnit = GL_TEXTURE0) const;
    void Unbind() const;

    GLuint GetTextureID() const { return textureID; }

private:
    GLuint textureID;
    int width;
    int height;
    int nrChannels;

    void LoadFromFile(const std::string &path);
};
