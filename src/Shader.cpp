#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// each shader file can have more than one type (vertex and fragment combiend)
Shader::Shader(const char *shaderFilePath)
{
    std::string source = ReadFile(shaderFilePath);
    auto preprocessedSources = PreProcess(source);

    // Compile and link shaders
    std::vector<GLuint> shaders;
    for (const auto &entry : preprocessedSources)
    {
        shaders.push_back(CompileShader(entry.second.c_str(), entry.first));
    }
    LinkProgram(shaders);

    // Cleanup
    for (auto shader : shaders)
    {
        glDeleteShader(shader);
    }
}

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath)
{
    std::string vertexSource = ReadFile(vertexShaderPath);
    std::string fragmentSource = ReadFile(fragmentShaderPath);

    // Compile shaders
    GLuint vertexShader = CompileShader(vertexSource.c_str(), GL_VERTEX_SHADER);
    GLuint fragmentShader = CompileShader(fragmentSource.c_str(), GL_FRAGMENT_SHADER);

    // Link program
    std::vector<GLuint> shaders = {vertexShader, fragmentShader};
    LinkProgram(shaders);

    // Cleanup
    for (auto shader : shaders)
    {
        glDeleteShader(shader);
    }
}

Shader::~Shader()
{
    glDeleteProgram(m_ID);
}

void Shader::UseProgram() const
{
    glUseProgram(m_ID);
}

void Shader::DeleteProgram() const
{
    glUseProgram(0);
}

void Shader::Bind() const
{
    glUseProgram(m_ID);
}

void Shader::UnBind() const
{
    glUseProgram(0);
}

std::string Shader::ReadFile(const char *shaderFilePath) const
{
    std::ifstream file(shaderFilePath, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open shader file: " + std::string(shaderFilePath));
    }

    std::stringstream buffer;
    buffer << file.rdbuf(); // Read file content
    file.close();
    return buffer.str();
}

// parse shader type 'vertex' or 'fragment' shaders
// parse source code from each shader type
std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string &source) const
{
    std::unordered_map<GLenum, std::string> shaderSources;
    const char *typeToken = "//type";
    size_t pos = source.find(typeToken, 0);
    while (pos != std::string::npos)
    {
        size_t eol = source.find_first_of("\r\n", pos);
        std::string type = source.substr(pos + strlen(typeToken) + 1, eol - pos - strlen(typeToken) - 1);

        GLenum shaderType;
        if (type == "vertex")
            shaderType = GL_VERTEX_SHADER;
        else if (type == "fragment")
            shaderType = GL_FRAGMENT_SHADER;
        else
            throw std::runtime_error("Unsupported shader type: " + type);

        size_t nextLinePos = source.find_first_not_of("\r\n", eol);
        pos = source.find(typeToken, nextLinePos);
        shaderSources[shaderType] = source.substr(nextLinePos, pos - nextLinePos);
    }

    return shaderSources;
}

GLuint Shader::CompileShader(const char *source, GLenum shaderType) const
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    CheckCompileErrors(shader, shaderType);
    std::cout << "[INFO]: Shader compiled with program ID: " << shader << "\n";

    return shader;
}

void Shader::LinkProgram(const std::vector<GLuint> &shaders)
{
    m_ID = glCreateProgram();
    for (auto shader : shaders)
    {
        glAttachShader(m_ID, shader);
    }
    glLinkProgram(m_ID);
    CheckLinkErrors(m_ID);
    std::cout << "[INFO]: Shader linked with ID: " << m_ID << "\n";
}

void Shader::CheckCompileErrors(GLuint shader, GLenum type) const
{
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        throw std::runtime_error("Shader compilation error (" + std::to_string(type) + "): " + infoLog);
    }
}

void Shader::CheckLinkErrors(GLuint program) const
{
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, 1024, nullptr, infoLog);
        throw std::runtime_error("Program linking error: " + std::string(infoLog));
    }
}

void Shader::UploadUniform1f(const std::string &name, const float &val)
{
    glUniform1f(GetUniformLocation(name), val);
}

void Shader::UploadUniform2f(const std::string &name, const glm::vec2 &val)
{
    glUniform2f(GetUniformLocation(name), val.x, val.y);
}

void Shader::UploadUniform3f(const std::string &name, const glm::vec3 &val)
{
    glUniform3f(GetUniformLocation(name), val.x, val.y, val.z);
}

void Shader::UploadUniform4f(const std::string &name, const glm::vec4 &val)
{
    glUniform4f(GetUniformLocation(name), val.x, val.y, val.z, val.w);
}

void Shader::UploadUniform1i(const std::string &name, const int &val)
{
    glUniform1i(GetUniformLocation(name), val);
}

void Shader::UploadUniform2i(const std::string &name, const glm::ivec2 &val)
{
    glUniform2i(GetUniformLocation(name), val.x, val.y);
}

void Shader::UploadUniform3i(const std::string &name, const glm::ivec3 &val)
{
    glUniform3i(GetUniformLocation(name), val.x, val.y, val.z);
}

void Shader::UploadUniform4i(const std::string &name, const glm::ivec4 &val)
{
    glUniform4i(GetUniformLocation(name), val.x, val.y, val.z, val.w);
}

void Shader::UploadUniformMat2(const std::string &name, const glm::mat2 &val)
{
    glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::UploadUniformMat3(const std::string &name, const glm::mat3 &val)
{
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::UploadUniformMat4(const std::string &name, const glm::mat4 &val)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(val));
}

GLint Shader::GetUniformLocation(const std::string &uniformName)
{
    // return from cache
    if (m_UniformLocationCache.find(uniformName) != m_UniformLocationCache.end())
        return m_UniformLocationCache[uniformName];

    auto location = glGetUniformLocation(m_ID, uniformName.c_str());
    if (location == -1)
        std::cout << "[SHADER-ID: " << m_ID << "] Uniform " << uniformName << " not found!" << "\n";
    m_UniformLocationCache[uniformName] = location;

    return location;
}

ShaderManager::ShaderManager()
    : m_ShaderDefaultPath("assets/shaders/")
{
}

ShaderManager::~ShaderManager()
{
    for (auto &shader : m_Shaders)
    {
        delete shader.second;
    }
}

void ShaderManager::AddShader(const std::string &shaderName, const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
    if (m_Shaders.find(shaderName) != m_Shaders.end())
    {
        std::cerr << "Shader already exists: " << shaderName << "\n";
        return;
    }
    std::string fullVertexShaderPath = m_ShaderDefaultPath + vertexShaderPath;
    std::string fullFragmentShaderPath = m_ShaderDefaultPath + fragmentShaderPath;

    std::cout << "[INFO]: Shader added from path: " << fullVertexShaderPath << "\n";
    std::cout << "[INFO]: Shader added from path: " << fullFragmentShaderPath << "\n";

    m_Shaders[shaderName] = new Shader(fullVertexShaderPath.c_str(), fullFragmentShaderPath.c_str());
    m_ShaderNames.push_back(shaderName);
}

void ShaderManager::RemoveShader(const std::string &shaderName)
{
    auto it = m_Shaders.find(shaderName);
    if (it == m_Shaders.end())
    {
        throw std::runtime_error("Shader with name " + shaderName + " not found.");
    }

    // Delete the shader and remove from the map
    delete it->second;
    m_Shaders.erase(it);

    // Remove the shader name from the list
    auto nameIt = std::find(m_ShaderNames.begin(), m_ShaderNames.end(), shaderName);
    if (nameIt != m_ShaderNames.end())
    {
        m_ShaderNames.erase(nameIt);
    }

    std::cout << "[INFO]: Shader Removed: " << shaderName << "\n";
}

void ShaderManager::UseShader(const std::string &shaderName)
{
    if (m_Shaders.find(shaderName) == m_Shaders.end())
    {
        throw std::runtime_error("Shader with name " + shaderName + " not found.");
    }
    m_Shaders[shaderName]->UseProgram();
}

Shader *ShaderManager::GetShader(const std::string &shaderName)
{
    auto it = m_Shaders.find(shaderName);
    if (it == m_Shaders.end())
    {
        throw std::runtime_error("Shader with name " + shaderName + " not found.");
    }
    return it->second;
}