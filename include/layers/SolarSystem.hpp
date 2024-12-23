#ifndef SOLAR_SYSTEM_LAYER_HPP
#define SOLAR_SYSTEM_LAYER_HPP

#include "Layer.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "Camera.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

struct CelestialBody
{
    glm::vec3 position;
    float size;
    float rotationSpeed;
    float axialTilt;
    float orbitalRadius;
    float orbitalSpeed;
};

class SolarSystemLayer : public Layer
{
public:
    SolarSystemLayer();
    virtual ~SolarSystemLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(float deltaTime) override;
    virtual void OnRender() override;
    virtual void OnEvent(Event &event) override;

    const inline std::vector<CelestialBody> &GetCelestialBodies() const
    {
        return m_CelestialBodies;
    }

private:
    void GenerateSphere(
        std::vector<float> &vertices,
        std::vector<unsigned int> &indices,
        float radius, int sectorCount, int stackCount);
    void GenerateOrbitLine(std::vector<float> &vertices, float radius, int segmentCount);

private:
    Camera m_Camera;
    ShaderManager m_ShaderManager;
    Shader *m_CurrentShader;

    float m_Time;
    float m_OrbitalSpeedScale;

    glm::mat4 m_Model;
    glm::mat4 m_View;
    glm::mat4 m_Projection;
    glm::vec3 m_LightPosition;
    glm::vec3 m_LightColor;

    GLuint m_VAO, m_VBO, m_EBO;
    GLuint m_OrbitLineVAO, m_OrbitLineVBO;

    std::vector<float> m_SphereVertices;
    std::vector<unsigned int> m_SphereIndices;
    std::vector<std::vector<float>> m_OrbitLineVertices;

    std::vector<CelestialBody> m_CelestialBodies;

    std::unique_ptr<Texture> m_CelestialTextures[10];
    std::unique_ptr<Texture> m_MoonTexture;
};

#endif
