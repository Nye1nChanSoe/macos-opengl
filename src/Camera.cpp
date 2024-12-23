#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "Application.hpp"

Camera::Camera()
    : m_Position(glm::vec3(0.0f, 0.0f, 70.0f)),
      m_Front(glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f))),
      m_Up(0.0f, 1.0f, 0.0f),
      m_Yaw(-50.0f),
      m_Pitch(0.0f),
      m_Speed(0.2f),
      m_RotationSpeed(0.6f),
      m_Sensitivity(0.05f)
{
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::ProcessInput()
{
    auto *window = Application::Get().GetWindow().GetNativeWindow();
    glm::vec3 Right = glm::normalize(glm::cross(m_Front, m_Up)); // Calculate right vector once

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_Position += m_Speed * m_Front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_Position -= m_Speed * m_Front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_Position -= m_Speed * Right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_Position += m_Speed * Right;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        m_Position += m_Speed * m_Up;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        m_Position -= m_Speed * m_Up;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        m_Yaw -= m_RotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        m_Yaw += m_RotationSpeed;

    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset)
{
    xOffset *= m_Sensitivity;
    yOffset *= m_Sensitivity;
    m_Yaw += xOffset;
    m_Pitch = glm::clamp(m_Pitch + yOffset, -89.0f, 89.0f);
    m_Front = glm::normalize(glm::vec3(
        cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
        sin(glm::radians(m_Pitch)),
        sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch))));
}
