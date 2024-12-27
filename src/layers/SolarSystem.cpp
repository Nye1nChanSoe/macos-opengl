#include "layers/SolarSystem.hpp"
#include "buffers/BufferLayout.hpp"
#include "Logger.hpp"
#include "Application.hpp"

SolarSystemLayer::SolarSystemLayer()
    : Layer("SolarSystemLayerLayer"), m_Time(0.0f), m_OrbitalSpeedScale(0.05f),
      m_CelestialBodies{
          {glm::vec3(0.0f, 0.0f, -20.0f), 50.0f, 2.0f, 7.25f, 0.0f, 0.0f},         // Sun
          {glm::vec3(0.0f, 0.0f, -50.0f), 0.38f, 10.0f, 0.03f, 30.0f, 47.87f},     // Mercury
          {glm::vec3(2.0f, 0.5f, -55.0f), 0.95f, 6.5f, 177.4f, 35.0f, 35.02f},     // Venus
          {glm::vec3(-2.5f, -0.5f, -65.0f), 1.0f, 15.0f, 23.44f, 45.0f, 29.78f},   // Earth
          {glm::vec3(3.5f, 1.0f, -75.0f), 0.53f, 13.0f, 25.19f, 55.0f, 24.07f},    // Mars
          {glm::vec3(-4.0f, 0.0f, -100.0f), 11.21f, 30.0f, 62.13f, 80.0f, 13.07f}, // Jupiter
          {glm::vec3(4.5f, -1.0f, -140.0f), 9.45f, 25.0f, 26.73f, 110.0f, 9.69f},  // Saturn
          {glm::vec3(-5.5f, 1.5f, -180.0f), 4.01f, 10.0f, 97.77f, 140.0f, 6.81f},  // Uranus
          {glm::vec3(5.5f, -0.5f, -200.0f), 3.88f, 12.0f, 28.32f, 160.0f, 5.43f},  // Neptune
          {glm::vec3(0.0f, 2.0f, -210.0f), 0.18f, 5.0f, 122.5f, 180.0f, 4.74f}     // Pluto
      }
{
    Logger::Debug("{} Added", m_DebugName);
}

SolarSystemLayer::~SolarSystemLayer()
{
    Logger::Debug("{} Removed", m_DebugName);
}

void SolarSystemLayer::OnAttach()
{
    Logger::Debug("{} Attached", m_DebugName);
    glEnable(GL_DEPTH_TEST);

    // Generate sphere data
    GenerateSphere(m_SphereVertices, m_SphereIndices, 0.5f, 36, 18);

    BufferLayout layout = {
        {BufferAttributeType::Vec3, "aPos"},
        {BufferAttributeType::Vec3, "aNormal"},
        {BufferAttributeType::Vec2, "aTexCoords"},
    };

    m_VAO = VertexArray::Create();
    m_VBO = VertexBuffer::Create(m_SphereVertices);
    m_EBO = IndexBuffer::Create(m_SphereIndices);
    m_VBO->SetLayout(layout);
    m_VAO->AddVertexBuffer(m_VBO);
    m_VAO->SetIndexBuffer(m_EBO);

    // Load textures for celestial bodies
    m_TextureManager = TextureManager::Create();
    m_TextureManager->AddTexture("sun", "sun.jpg");
    m_TextureManager->AddTexture("mercury", "mercury.jpg");
    m_TextureManager->AddTexture("venus", "venus.jpeg");
    m_TextureManager->AddTexture("earth", "earth.jpg");
    m_TextureManager->AddTexture("mars", "mars.jpg");
    m_TextureManager->AddTexture("jupiter", "jupiter.jpg");
    m_TextureManager->AddTexture("saturn", "saturn.jpg");
    m_TextureManager->AddTexture("uranus", "uranus.jpg");
    m_TextureManager->AddTexture("neptune", "neptune.jpg");
    m_TextureManager->AddTexture("pluto", "pluto.jpg");
    m_TextureManager->AddTexture("moon", "moon.jpg");

    m_TextureNames = m_TextureManager->GetTextureNames();

    // Load shaders
    m_ShaderManager = ShaderManager::Create();
    m_ShaderManager->AddShader("SolarSystemPhong", "phong_vertex_shader.glsl", "phong_frag_shader.glsl");
    m_ShaderManager->AddShader("SolarSystemGouraud", "gouraud_vertex_shader.glsl", "gouraud_frag_shader.glsl");
    m_ShaderManager->AddShader("SolarSystemFlat", "flat_vertex_shader.glsl", "flat_frag_shader.glsl");

    m_Model = glm::mat4(1.0f);
    m_View = m_Camera.GetViewMatrix();
    m_Projection = glm::perspective(
        glm::radians(60.0f), Application::Get().GetWindow().GetAspectRatio(), 0.1f, 1000.0f);
    m_LightPosition = m_CelestialBodies.at(0).position;
    m_LightColor = glm::vec3(1.0f, 1.0f, 0.8f); // Bright white-yellow light
}

void SolarSystemLayer::OnDetach()
{
    Logger::Debug("{} Detached", m_DebugName);
    m_VBO->UnBind();
    m_EBO->UnBind();
    m_VAO->UnBind();
}

void SolarSystemLayer::OnUpdate(float deltaTime)
{
    m_Time += deltaTime;
    m_Camera.ProcessInput();
}

void SolarSystemLayer::OnEvent(Event &event)
{
    m_Camera.ProcessInput();
}

void SolarSystemLayer::OnRender()
{
    m_View = m_Camera.GetViewMatrix();

    auto *shader = m_ShaderManager->GetShader("SolarSystemPhong");
    shader->UseProgram();
    shader->UploadUniformMat4("model", m_Model);
    shader->UploadUniformMat4("view", m_View);
    shader->UploadUniformMat4("projection", m_Projection);

    shader->UploadUniform3f("lightPos", m_LightPosition);
    shader->UploadUniform3f("viewPos", m_Camera.GetPosition());
    shader->UploadUniform3f("lightColor", m_LightColor);

    for (unsigned int i = 0; i < m_CelestialBodies.size(); i++)
    {
        m_Model = glm::mat4(1.0f);
        if (i > 0)
        {
            float angle = m_Time * glm::radians(m_CelestialBodies.at(i).orbitalSpeed * m_OrbitalSpeedScale);
            float x = m_CelestialBodies.at(i).orbitalRadius * cos(angle);
            float z = m_CelestialBodies.at(i).orbitalRadius * sin(angle);
            m_Model = glm::translate(m_Model, glm::vec3(x, 0.0f, z));
        }

        m_Model = glm::scale(m_Model, glm::vec3(m_CelestialBodies.at(i).size));
        m_Model = glm::rotate(m_Model, glm::radians(m_CelestialBodies.at(i).axialTilt), glm::vec3(1.0f, 0.0f, 0.0f));
        m_Model = glm::rotate(m_Model, glm::radians(m_Time * m_CelestialBodies.at(i).rotationSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
        shader->UploadUniformMat4("model", m_Model);
        shader->UploadUniform1i("isSun", (i == 0) ? 1 : 0);

        Texture *texture = m_TextureManager->GetTexture(m_TextureNames.at(i));
        texture->Bind();

        m_VAO->Bind();
        glDrawElements(GL_TRIANGLES, m_EBO->GetCount(), GL_UNSIGNED_INT, 0);
        m_VAO->UnBind();

        // Render moon orbiting Earth
        if (i == 3) // Earth index
        {
            glm::mat4 moonModel = m_Model;
            float moonAngle = m_Time * glm::radians(40.0f * m_OrbitalSpeedScale);
            float x = 2.0f * cos(moonAngle);
            float z = 2.0f * sin(moonAngle);
            moonModel = glm::translate(moonModel, glm::vec3(x, 0.0f, z));
            moonModel = glm::scale(moonModel, glm::vec3(0.27f));

            shader->UploadUniformMat4("model", moonModel);
            shader->UploadUniform1i("isSun", (i == 0) ? 1 : 0);

            Texture *texture = m_TextureManager->GetTexture("moon");
            texture->Bind();

            m_VAO->Bind();
            glDrawElements(GL_TRIANGLES, m_EBO->GetCount(), GL_UNSIGNED_INT, 0);
            m_VAO->UnBind();
        }
    }
    shader->UnBind();
}

void SolarSystemLayer::GenerateSphere(std::vector<float> &vertices, std::vector<unsigned int> &indices, float radius, int sectorCount, int stackCount)
{
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);

            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            vertices.push_back(s);
            vertices.push_back(t);
        }
    }

    for (int i = 0; i < stackCount; ++i)
    {
        int k1 = i * (sectorCount + 1);
        int k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

void SolarSystemLayer::GenerateOrbitLine(std::vector<float> &vertices, float radius, int segmentCount)
{
    float angleStep = 2 * M_PI / segmentCount;

    for (int i = 0; i <= segmentCount; ++i)
    {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        vertices.push_back(x);
        vertices.push_back(0.0f); // y is 0 for the orbit plane
        vertices.push_back(z);
    }
}
