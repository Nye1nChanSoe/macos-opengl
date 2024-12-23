#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <random>
#include <memory>

#include "Window.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "ImGuiHandler.hpp"
#include "Time.hpp"
#include "events/KeyEvent.hpp"
#include "events/WindowEvent.hpp"
#include "events/MouseEvent.hpp"

void GenerateSphere(std::vector<float> &vertices, std::vector<unsigned int> &indices, float radius, int sectorCount, int stackCount)
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

// Constants for celestial body properties
struct CelestialBody
{
    glm::vec3 position;
    float size;
    float rotationSpeed;
    float axialTilt;
    float orbitalRadius;
    float orbitalSpeed;
};

const CelestialBody celestialBodies[] = {
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
};
float orbitalSpeedScale = 0.1f; // Adjust this for smooth visuals

const float moonOrbitRadius = 2.0f;
const float moonOrbitSpeed = 40.0f;
const float moonSize = 0.27f;

int main()
{
    Window::Properties props = {"macos-opengl", 1000, 740, true, [](Event &e)
                                {
                                    if (e.GetType() == Event::Type::KeyPressed)
                                    {
                                        auto &keyEvent = static_cast<KeyPressedEvent &>(e);
                                        std::cout << "Key Pressed: " << keyEvent.GetKeyCode() << "\n";
                                    }
                                    if (e.GetType() == Event::Type::MouseMoved)
                                    {
                                        auto &mouseEvent = static_cast<MouseMovedEvent &>(e);
                                        std::cout << "Mouse Moved: " << mouseEvent.GetX() << "\n";
                                    }
                                }};
    Window window(props);

    // Initialize ImGui
    ImGuiHandler imguiHandler(window.GetNativeWindow());
    imguiHandler.Initialize();

    float vertices[] = {
        // Position coords  // Color coords // Texture coords
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        // Left face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,

        // Right face
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        // Top face
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    unsigned int indices[] = {
        // Back face
        0, 1, 2, 2, 3, 0,
        // Front face
        4, 5, 6, 6, 7, 4,
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        12, 13, 14, 14, 15, 12,
        // Top face
        16, 17, 18, 18, 19, 16,
        // Bottom face
        20, 21, 22, 22, 23, 20};

    std::vector<float> sphereVertices;
    std::vector<unsigned int> sphereIndices;
    GenerateSphere(sphereVertices, sphereIndices, 0.5f, 36, 18);

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), sphereIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0); // Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float))); // Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float))); // Texture Coords
    glEnableVertexAttribArray(2);

    glEnable(GL_DEPTH_TEST);

    std::unique_ptr<Texture> celestialTextures[10];
    celestialTextures[0] = std::make_unique<Texture>("assets/textures/sun.jpg");
    celestialTextures[1] = std::make_unique<Texture>("assets/textures/mercury.jpg");
    celestialTextures[2] = std::make_unique<Texture>("assets/textures/venus.jpeg");
    celestialTextures[3] = std::make_unique<Texture>("assets/textures/earth.jpg");
    celestialTextures[4] = std::make_unique<Texture>("assets/textures/mars.jpg");
    celestialTextures[5] = std::make_unique<Texture>("assets/textures/jupiter.jpg");
    celestialTextures[6] = std::make_unique<Texture>("assets/textures/saturn.jpg");
    celestialTextures[7] = std::make_unique<Texture>("assets/textures/uranus.jpg");
    celestialTextures[8] = std::make_unique<Texture>("assets/textures/neptune.jpg");
    celestialTextures[9] = std::make_unique<Texture>("assets/textures/pluto.jpg");
    std::unique_ptr<Texture> moonTexture = std::make_unique<Texture>("assets/textures/moon.jpg");

    ShaderManager shaderLib;
    shaderLib.AddShader("SolarSystemPhong", "phong_vertex_shader.glsl", "phong_frag_shader.glsl");
    shaderLib.AddShader("SolarSystemGouraud", "gouraud_vertex_shader.glsl", "gouraud_frag_shader.glsl");
    shaderLib.AddShader("SolarSystemFlat", "flat_vertex_shader.glsl", "flat_frag_shader.glsl");
    Shader *shaderPhong = shaderLib.GetShader("SolarSystemPhong");
    Shader *shaderGouraud = shaderLib.GetShader("SolarSystemGouraud");
    Shader *shaderFlat = shaderLib.GetShader("SolarSystemFlat");

    auto shaderNames = shaderLib.GetShaderNames();
    int selectedShader = 0; // Default shader index

    Camera camera;
    Time time;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
    glm::vec3 lightPosition = celestialBodies[0].position; // Sun's position
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 0.8f);    // Bright white-yellow light

    // KeyPressedEvent KeyEvent(65, false);
    // WindowResizedEvent resizeEvent(800, 600);
    // MouseMovedEvent mouseMoveEvent(100.0f, 200.0f);

    // std::cout << KeyEvent << std::endl;
    // std::cout << resizeEvent << std::endl;
    // std::cout << mouseMoveEvent << std::endl;

    while (!glfwWindowShouldClose(window.GetNativeWindow()))
    {
        double currentTime = glfwGetTime();
        time.Update(currentTime);

        if (glfwGetKey(window.GetNativeWindow(), GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(window.GetNativeWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window.GetNativeWindow(), true);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.ProcessInput(window.GetNativeWindow());
        view = camera.GetViewMatrix();

        Shader *currentShader = shaderLib.GetShader(shaderNames[selectedShader]);
        currentShader->UseProgram();

        currentShader->UploadUniformMat4("model", model);
        currentShader->UploadUniformMat4("view", view);
        currentShader->UploadUniformMat4("projection", projection);

        currentShader->UploadUniform3f("lightPos", lightPosition);
        currentShader->UploadUniform3f("viewPos", camera.Position);
        currentShader->UploadUniform3f("lightColor", lightColor);

        for (unsigned int i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            if (i > 0)
            {
                float angle = glfwGetTime() * glm::radians(celestialBodies[i].orbitalSpeed * orbitalSpeedScale);
                float x = celestialBodies[i].orbitalRadius * cos(angle);
                float z = celestialBodies[i].orbitalRadius * sin(angle);
                model = glm::translate(model, glm::vec3(x, 0.0f, z));
            }

            model = glm::scale(model, glm::vec3(celestialBodies[i].size));
            model = glm::rotate(model, glm::radians(celestialBodies[i].axialTilt), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(static_cast<float>(glfwGetTime()) * celestialBodies[i].rotationSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
            currentShader->UploadUniformMat4("model", model);
            currentShader->UploadUniform1i("isSun", (i == 0) ? 1 : 0);

            celestialTextures[i]->Bind();
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);

            // for moon orbiting earth
            if (i == 3)
            {
                glm::mat4 moonModel = model;
                float moonAngle = glfwGetTime() * glm::radians(moonOrbitSpeed);
                float x = moonOrbitRadius * cos(moonAngle);
                float z = moonOrbitRadius * sin(moonAngle);
                moonModel = glm::translate(moonModel, glm::vec3(x, 0.0f, z));
                moonModel = glm::scale(moonModel, glm::vec3(moonSize));

                currentShader->UploadUniformMat4("model", moonModel);
                currentShader->UploadUniform1i("isSun", 0);

                moonTexture->Bind();
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
            }
        }

        imguiHandler.BeginFrame();
        imguiHandler.RenderShaderSelector(shaderNames, selectedShader);
        imguiHandler.RenderDebugWindow(time);
        imguiHandler.EndFrame();

        window.OnUpdate();
    }

    shaderLib.RemoveShader("SolarSystemPhong");
    shaderLib.RemoveShader("SolarSystemGouraud");
    shaderLib.RemoveShader("SolarSystemFlat");
    imguiHandler.Cleanup();
    window.Shutdown();

    return 0;
}
