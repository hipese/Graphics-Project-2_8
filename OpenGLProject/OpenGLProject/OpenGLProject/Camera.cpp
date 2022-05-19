#include "Camera.h"
#include "Actor.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_zoom(ZOOM)
{
    m_position = position;
    m_worldUp = up;
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_zoom(ZOOM)
{
    m_position = glm::vec3(posX, posY, posZ);
    m_worldUp = glm::vec3(upX, upY, upZ);
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();
}

void Camera::OnUpdate()
{
}

glm::mat4 Camera::GetViewMatrix()
{
    glm::vec3 eye = m_position;
    glm::vec3 center = m_position + m_front;
    if (m_parent != nullptr)
    {
        eye += m_parent->GetPosition();
        center += m_parent->GetPosition();
    }
    return glm::lookAt(eye, center, m_up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;
    if (direction == Camera_Movement::FORWARD)
        m_position += m_front * velocity;
    if (direction == Camera_Movement::BACKWARD)
        m_position -= m_front * velocity;
    if (direction == Camera_Movement::LEFT)
        m_position -= m_right * velocity;
    if (direction == Camera_Movement::RIGHT)
        m_position += m_right * velocity;
    if (direction == Camera_Movement::UP)
        m_position += m_worldUp * velocity;
    if (direction == Camera_Movement::DOWN)
        m_position -= m_worldUp * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    // pitch가 범위를 벗어났을 때 화면이 뒤집히는 것을 방지
    if (constrainPitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    // 전면, 오른쪽 및 위쪽 벡터 업데이트
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    m_zoom -= (float)yoffset;
    if (m_zoom < 1.0f)
        m_zoom = 1.0f;
    if (m_zoom > 45.0f)
        m_zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_front));
}