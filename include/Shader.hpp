#ifndef SHADER_HPP
#define SHADER_HPP

#define GL_SILENCE_DEPRECATION

#include <string>
#include <unordered_map>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

class Shader
{
private:
    GLuint m_ID; // Shader Program's ID used by OpenGL
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    // will parse the #type prefix used to write in single file for shaders
    Shader(const char *shaderFilePath);
    Shader(
        const char *vertexShaderPath,
        const char *fragmentShaderPath);
    ~Shader();

    void Bind() const;
    void UseProgram() const;
    void UnBind() const;
    void DeleteProgram() const;

    void UploadUniform1f(const std::string &name, const float &val);
    void UploadUniform2f(const std::string &name, const glm::vec2 &val);
    void UploadUniform3f(const std::string &name, const glm::vec3 &val);
    void UploadUniform4f(const std::string &name, const glm::vec4 &val);

    void UploadUniform1i(const std::string &name, const int &val);
    void UploadUniform2i(const std::string &name, const glm::ivec2 &val);
    void UploadUniform3i(const std::string &name, const glm::ivec3 &val);
    void UploadUniform4i(const std::string &name, const glm::ivec4 &val);

    void UploadUniformMat2(const std::string &name, const glm::mat2 &val);
    void UploadUniformMat3(const std::string &name, const glm::mat3 &val);
    void UploadUniformMat4(const std::string &name, const glm::mat4 &val);

    GLint inline getID() const { return m_ID; }

private:
    // read, compile, link, debug
    std::string ReadFile(const char *shaderFilePath) const;
    std::unordered_map<GLenum, std::string> PreProcess(const std::string &source) const;
    GLuint CompileShader(const char *source, GLenum shaderType) const;
    void LinkProgram(const std::vector<GLuint> &shaders);
    void CheckCompileErrors(GLuint shader, GLenum type) const;
    void CheckLinkErrors(GLuint program) const;

    GLint GetUniformLocation(const std::string &uniformName);
};

class ShaderManager
{
private:
    std::unordered_map<std::string, Shader *> m_Shaders;
    std::unordered_map<std::string, GLuint> m_ShaderCache;
    std::string m_ShaderDefaultPath;
    std::vector<std::string> m_ShaderNames;

public:
    ShaderManager();
    ~ShaderManager();

    void AddShader(const std::string &shaderName, const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
    void UseShader(const std::string &shaderName);
    void RemoveShader(const std::string &shaderName);

    void inline SetShaderDefaultPath(const std::string &defaultPath) { m_ShaderDefaultPath = defaultPath; }

    Shader *GetShader(const std::string &shaderName);

    std::vector<std::string> inline GetShaderNames() const { return m_ShaderNames; }

    void ClearShaders();
};

#endif