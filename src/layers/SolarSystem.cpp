#include "layers/SolarSystem.hpp"
#include "Application.hpp"
#include "Logger.hpp"

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
    // Generate sphere data
    GenerateSphere(m_SphereVertices, m_SphereIndices, 0.5f, 36, 18);

    // Setup VAO, VBO, EBO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    // after this binding VAO will remember everything underneath
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_SphereVertices.size() * sizeof(float), m_SphereVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_SphereIndices.size() * sizeof(unsigned int), m_SphereIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0); // Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float))); // Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float))); // Texture Coords
    glEnableVertexAttribArray(2);

    // Unbind VAO for safety
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    // Load textures for celestial bodies
    m_CelestialTextures[0] = std::make_unique<Texture>("assets/textures/sun.jpg");
    m_CelestialTextures[1] = std::make_unique<Texture>("assets/textures/mercury.jpg");
    m_CelestialTextures[2] = std::make_unique<Texture>("assets/textures/venus.jpeg");
    m_CelestialTextures[3] = std::make_unique<Texture>("assets/textures/earth.jpg");
    m_CelestialTextures[4] = std::make_unique<Texture>("assets/textures/mars.jpg");
    m_CelestialTextures[5] = std::make_unique<Texture>("assets/textures/jupiter.jpg");
    m_CelestialTextures[6] = std::make_unique<Texture>("assets/textures/saturn.jpg");
    m_CelestialTextures[7] = std::make_unique<Texture>("assets/textures/uranus.jpg");
    m_CelestialTextures[8] = std::make_unique<Texture>("assets/textures/neptune.jpg");
    m_CelestialTextures[9] = std::make_unique<Texture>("assets/textures/pluto.jpg");
    m_MoonTexture = std::make_unique<Texture>("assets/textures/moon.jpg");

    // Load shaders
    m_ShaderManager.AddShader("SolarSystemPhong", "phong_vertex_shader.glsl", "phong_frag_shader.glsl");
    m_ShaderManager.AddShader("SolarSystemGouraud", "gouraud_vertex_shader.glsl", "gouraud_frag_shader.glsl");
    m_ShaderManager.AddShader("SolarSystemFlat", "flat_vertex_shader.glsl", "flat_frag_shader.glsl");

    m_CurrentShader = m_ShaderManager.GetShader("SolarSystemPhong");

    m_Model = glm::mat4(1.0f);
    m_View = m_Camera.GetViewMatrix();
    m_Projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
    m_LightPosition = m_CelestialBodies.at(0).position;
    m_LightColor = glm::vec3(1.0f, 1.0f, 0.8f); // Bright white-yellow light
}

void SolarSystemLayer::OnDetach()
{
    Logger::Debug("{} Detached", m_DebugName);
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
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

    m_CurrentShader->UseProgram();
    m_CurrentShader->UploadUniformMat4("model", m_Model);
    m_CurrentShader->UploadUniformMat4("view", m_View);
    m_CurrentShader->UploadUniformMat4("projection", m_Projection);

    m_CurrentShader->UploadUniform3f("lightPos", m_LightPosition);
    m_CurrentShader->UploadUniform3f("viewPos", m_Camera.GetPosition());
    m_CurrentShader->UploadUniform3f("lightColor", m_LightColor);

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
        m_CurrentShader->UploadUniformMat4("model", m_Model);
        m_CurrentShader->UploadUniform1i("isSun", (i == 0) ? 1 : 0);

        m_CelestialTextures[i]->Bind();
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, m_SphereIndices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Render moon orbiting Earth
        if (i == 3) // Earth index
        {
            glm::mat4 moonModel = m_Model;
            float moonAngle = m_Time * glm::radians(40.0f * m_OrbitalSpeedScale);
            float x = 2.0f * cos(moonAngle);
            float z = 2.0f * sin(moonAngle);
            moonModel = glm::translate(moonModel, glm::vec3(x, 0.0f, z));
            moonModel = glm::scale(moonModel, glm::vec3(0.27f));

            m_CurrentShader->UploadUniformMat4("model", moonModel);
            m_CurrentShader->UploadUniform1i("isSun", (i == 0) ? 1 : 0);
            m_MoonTexture->Bind();
            glBindVertexArray(m_VAO);
            glDrawElements(GL_TRIANGLES, m_SphereIndices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }
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
