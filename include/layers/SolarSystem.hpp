#ifndef SOLAR_SYSTEM_LAYER_HPP
#define SOLAR_SYSTEM_LAYER_HPP

#include "Layer.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "Camera.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include "buffers/VertexBuffer.hpp"
#include "buffers/IndexBuffer.hpp"
#include "buffers/VertexArray.hpp"

#include "events/MouseEvent.hpp"

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

    // Eventhandlers
    bool OnMouseMove(MouseMovedEvent &e);

private:
    Camera m_Camera;
    std::unique_ptr<ShaderManager> m_ShaderManager;
    std::unique_ptr<TextureManager> m_TextureManager;

    float m_Time;
    float m_OrbitalSpeedScale;

    glm::mat4 m_Model;
    glm::mat4 m_View;
    glm::mat4 m_Projection;
    glm::vec3 m_LightPosition;
    glm::vec3 m_LightColor;

    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<IndexBuffer> m_EBO;
    std::shared_ptr<VertexArray> m_VAO;
    std::shared_ptr<VertexArray> m_OrbitVAO;
    std::shared_ptr<VertexBuffer> m_OrbitVBO;

    std::vector<float> m_SphereVertices;
    std::vector<float> m_OrbitLineVertices;
    std::vector<unsigned int> m_SphereIndices;

    std::vector<CelestialBody> m_CelestialBodies;
    std::vector<std::string> m_TextureNames;

    // Camera Mouse Movements
    // TODO: implement mouse movements
    float m_LastMouseX = 0.0f;
    float m_LastMouseY = 0.0f;
    bool m_FirstMouseMove = true;
};

#endif
