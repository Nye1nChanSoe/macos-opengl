#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Shader.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include <vector>
#include <random>
#include <memory>

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

void GenerateDisk(std::vector<float> &vertices, std::vector<unsigned int> &indices, float innerRadius, float outerRadius, int sectorCount)
{
    float sectorStep = 2 * M_PI / sectorCount;
    float sectorAngle;

    // Generate vertices
    for (int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        float cosAngle = cosf(sectorAngle);
        float sinAngle = sinf(sectorAngle);

        // Outer ring vertex
        vertices.push_back(outerRadius * cosAngle);
        vertices.push_back(0.0f);
        vertices.push_back(outerRadius * sinAngle);
        vertices.push_back((cosAngle + 1) / 2);
        vertices.push_back((sinAngle + 1) / 2);

        // Inner ring vertex
        vertices.push_back(innerRadius * cosAngle);
        vertices.push_back(0.0f);
        vertices.push_back(innerRadius * sinAngle);
        vertices.push_back((cosAngle + 1) / 2);
        vertices.push_back((sinAngle + 1) / 2);
    }

    // Generate indices
    for (int i = 0; i < sectorCount; ++i)
    {
        int k1 = i * 2;
        int k2 = k1 + 2;

        indices.push_back(k1);
        indices.push_back(k2);
        indices.push_back(k1 + 1);

        indices.push_back(k2);
        indices.push_back(k2 + 1);
        indices.push_back(k1 + 1);
    }
}

int main()
{
    if (!glfwInit())
        return -1;

    // OpenGL context hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(900, 700, "macos-opengl", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

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

    // Positions: The Sun at the center, planets follow
    glm::vec3 celestialPositions[] = {
        glm::vec3(0.0f, 0.0f, -20.0f),    // Sun (closer to camera)
        glm::vec3(0.0f, 0.0f, -50.0f),    // Mercury
        glm::vec3(2.0f, 0.5f, -55.0f),    // Venus
        glm::vec3(-2.5f, -0.5f, -65.0f),  // Earth
        glm::vec3(3.5f, 1.0f, -75.0f),    // Mars
        glm::vec3(-4.0f, 0.0f, -100.0f),  // Jupiter
        glm::vec3(4.5f, -1.0f, -140.0f),  // Saturn
        glm::vec3(-5.5f, 1.5f, -1800.0f), // Uranus
        glm::vec3(5.5f, -0.5f, -200.0f),  // Neptune
        glm::vec3(0.0f, 2.0f, -210.0f)    // Pluto
    };

    // Sizes: The Sun is much larger
    float celestialSizes[] = {
        50.0f,  // Sun
        0.38f,  // Mercury
        0.95f,  // Venus
        1.0f,   // Earth
        0.53f,  // Mars
        11.21f, // Jupiter
        9.45f,  // Saturn
        4.01f,  // Uranus
        3.88f,  // Neptune
        0.18f   // Pluto
    };

    // Rotation speeds (degrees/second): Sun rotates slowly
    float celestialRotationSpeeds[] = {
        2.0f,  // Sun
        10.0f, // Mercury
        6.5f,  // Venus
        15.0f, // Earth
        13.0f, // Mars
        30.0f, // Jupiter
        25.0f, // Saturn
        10.0f, // Uranus (tilted)
        12.0f, // Neptune
        5.0f   // Pluto
    };

    float celestialAxialTilts[] = {
        7.25f,  // Sun
        0.03f,  // Mercury
        177.4f, // Venus (tilted nearly upside down)
        23.44f, // Earth
        25.19f, // Mars
        62.13f, // Jupiter
        26.73f, // Saturn
        97.77f, // Uranus (extreme tilt)
        28.32f, // Neptune
        122.5f  // Pluto
    };

    // Orbital radii (distance from Sun in relative units)
    float orbitalRadii[] = {
        0.0f,   // Sun (no orbit)
        30.0f,  // Mercury
        35.0f,  // Venus
        45.0f,  // Earth
        55.0f,  // Mars
        80.0f,  // Jupiter
        110.0f, // Saturn
        140.0f, // Uranus
        160.0f, // Neptune
        180.0f  // Pluto
    };

    // Orbital speeds (degrees per second, faster for closer planets)
    float speedScale = 0.1f; // Adjust this for smooth visuals
    float orbitalSpeeds[] = {
        0.0f,                // Sun (no orbit)
        47.87f * speedScale, // Mercury (fastest orbit)
        35.02f * speedScale, // Venus
        29.78f * speedScale, // Earth
        24.07f * speedScale, // Mars
        13.07f * speedScale, // Jupiter
        9.69f * speedScale,  // Saturn
        6.81f * speedScale,  // Uranus
        5.43f * speedScale,  // Neptune
        4.74f * speedScale   // Pluto (slowest orbit)
    };

    std::unique_ptr<Texture> moonTexture = std::make_unique<Texture>("assets/textures/moon.jpg");
    float moonOrbitRadius = 1.5f; // Distance from Earth
    float moonOrbitSpeed = 50.0f; // Faster orbit around Earth
    float moonSize = 0.27f;       // Moon size relative to Earth

    std::vector<float> ringVertices;
    std::vector<unsigned int> ringIndices;
    GenerateDisk(ringVertices, ringIndices, 1.5f, 2.5f, 100);

    Shader shader("assets/shaders/vertex_shader.glsl", "assets/shaders/fragment_shader.glsl");
    Camera camera;

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

    while (!glfwWindowShouldClose(window))
    {
        camera.ProcessInput(window);

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();

        glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, -20.0f); // Sun's position
        shader.use();

        shader.setVec3("lightPos", lightPos);
        shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 0.8f)); // Bright white-yellow light
        shader.setVec3("viewPos", camera.Position);
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);

            if (i > 0) // For planets
            {
                float angle = glfwGetTime() * glm::radians(orbitalSpeeds[i]); // Orbit angle over time
                float x = orbitalRadii[i] * cos(angle);
                float z = orbitalRadii[i] * sin(angle);
                model = glm::translate(model, glm::vec3(x, 0.0f, z));
            }
            else // For Sun
            {
                model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            }

            model = glm::rotate(model, glm::radians(celestialAxialTilts[i]), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(static_cast<float>(glfwGetTime()) * celestialRotationSpeeds[i]), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(celestialSizes[i]));
            shader.setMat4("model", glm::value_ptr(model));

            if (i == 0) // Sun's own light emission
            {
                shader.setInt("isSun", 1);
            }
            else
            {
                shader.setInt("isSun", 0);
            }

            celestialTextures[i]->Bind();
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);

            if (i == 3) // Moon orbiting Earth
            {
                glm::mat4 moonModel = model;
                float moonAngle = glfwGetTime() * glm::radians(40.0f); // Moon's orbit speed
                float moonOrbitRadius = 2.0f;                          // Distance from Earth
                float x = moonOrbitRadius * cos(moonAngle);
                float z = moonOrbitRadius * sin(moonAngle);
                moonModel = glm::translate(moonModel, glm::vec3(x, 0.0f, z));
                moonModel = glm::scale(moonModel, glm::vec3(0.27f)); // Moon size

                shader.setMat4("model", glm::value_ptr(moonModel));
                shader.setInt("isSun", 0);
                moonTexture->Bind();
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
