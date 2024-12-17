#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position)
    : Position(glm::vec3(-80.0f, 20.0f, -50.0f)),
      Front(glm::normalize(glm::vec3(10.0f, -0.2f, -1.0f))),
      Up(0.0f, 1.0f, 0.0f),
      Yaw(-20.0f),
      Pitch(-15.0f),
      Speed(0.2f),
      RotationSpeed(0.6f),
      Sensitivity(0.05f)
{
    // move front
    // this->Position += 20.0f * glm::normalize(glm::vec3(10.0f, -0.2f, -1.0f));
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessInput(GLFWwindow *window)
{
    glm::vec3 Right = glm::normalize(glm::cross(Front, Up)); // Calculate right vector once

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Position += Speed * Front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Position -= Speed * Front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Position -= Speed * Right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Position += Speed * Right;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        Position += Speed * Up;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        Position -= Speed * Up;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        Yaw -= RotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        Yaw += RotationSpeed;

    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset)
{
    xOffset *= Sensitivity;
    yOffset *= Sensitivity;
    Yaw += xOffset;
    Pitch = glm::clamp(Pitch + yOffset, -89.0f, 89.0f);
    Front = glm::normalize(glm::vec3(
        cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)),
        sin(glm::radians(Pitch)),
        sin(glm::radians(Yaw)) * cos(glm::radians(Pitch))));
}
