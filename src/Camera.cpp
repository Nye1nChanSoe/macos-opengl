#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position)
    : Position(position), Front(0.0f, 0.0f, -1.0f), Up(0.0f, 1.0f, 0.0f),
      Yaw(-90.0f), Pitch(0.0f), Speed(0.05f), Sensitivity(0.1f) {}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Position += Speed * Front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Position -= Speed * Front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Position -= glm::normalize(glm::cross(Front, Up)) * Speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Position += glm::normalize(glm::cross(Front, Up)) * Speed;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        Position += Speed * Up;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        Position -= Speed * Up;
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
