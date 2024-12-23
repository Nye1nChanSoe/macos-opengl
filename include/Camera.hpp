#ifndef CAMERA_HPP
#define CAMERA_HPP

#define GL_SILENCE_DEPRECATION

#include <glm/glm.hpp>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

class Camera
{
public:
    Camera();
    ~Camera() = default;

    glm::mat4 GetViewMatrix() const;
    void ProcessInput();
    void ProcessMouseMovement(float xOffset, float yOffset);

    const inline glm::vec3 &GetPosition() const { return m_Position; }
    const inline glm::vec3 &GetFront() const { return m_Front; }
    const inline glm::vec3 &GetUp() const { return m_Up; }
    const inline glm::vec3 &GetRight() const { return m_Right; }
    float inline GetPitch() const { return m_Pitch; }
    float inline GetYaw() const { return m_Yaw; }
    float inline GetSpeed() const { return m_Speed; }
    float inline GetRotationSpeed() const { return m_RotationSpeed; }
    float inline GetSensitivity() const { return m_Sensitivity; }

    void SetPosition(const glm::vec3 &position) { m_Position = position; }
    void SetFront(const glm::vec3 &front) { m_Front = front; }
    void SetYaw(float yaw) { m_Yaw = yaw; }
    void SetPitch(float pitch) { m_Pitch = pitch; }
    void SetSpeed(float speed) { m_Speed = speed; }
    void SetRotationSpeed(float rotationSpeed) { m_RotationSpeed = rotationSpeed; }
    void SetSensitivity(float sensitivity) { m_Sensitivity = sensitivity; }

private:
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;

    float m_Yaw;
    float m_Pitch;
    float m_Speed;
    float m_RotationSpeed;
    float m_Sensitivity;
};

#endif
