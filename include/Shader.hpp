#pragma once

#define GL_SILENCE_DEPRECATION

#include <string>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
    GLuint ID;
    Shader(const char *vertexPath, const char *fragmentPath);
    void use() const;
    void setMat4(const std::string &name, const float *value) const;
    void setInt(const std::string &name, int value) const;
};
