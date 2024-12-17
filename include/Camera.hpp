#pragma once

#define GL_SILENCE_DEPRECATION

#include <glm/glm.hpp>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

class Camera
{
public:
    glm::vec3 Position, Front, Up;
    float Yaw, Pitch, Speed, Sensitivity;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f));
    glm::mat4 GetViewMatrix() const;
    void ProcessInput(GLFWwindow *window);
    void ProcessMouseMovement(float xOffset, float yOffset);
};
