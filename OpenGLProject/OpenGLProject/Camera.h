#pragma once
#include "Transform.h"

// ī�޶� �̵��� ���� �� ���� ������ �ɼ��� ����
enum class Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// ī�޶��� �⺻ ����
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 30.0f;

// �Է��� ó���ϰ� OpenGL���� ����� �ش� ���Ϸ� ����, ���� �� ����� ����ϴ� ī�޶� Ŭ����
class Camera : public Transform
{
public:
    // ī�޶� �Ӽ�
    glm::vec3 m_front{};
    glm::vec3 m_up{};
    glm::vec3 m_right{};
    glm::vec3 m_worldUp{};
    // ���Ϸ� ����
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;
    // ī�޶� ����
    float m_movementSpeed = 0.0f;
    float m_mouseSensitivity = 0.0f;
    float m_zoom = 0.0f;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    
    // ���Ϸ� ���� �� LookAt ��ĸ� ����Ͽ� ���� �� ����� ��ȯ
    glm::mat4 GetViewMatrix();
    // ���콺 �Է� �ý��ۿ��� ���� �Է��� ó��
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    // ���콺 �� ��ũ�� �̺�Ʈ
    void ProcessMouseScroll(float yoffset);
    // ������ ���� ������Ʈ
    void OnUpdate();
    // ���� ���� ��ȯ
    inline glm::vec3 GetFront(){return m_front;}
    inline float GetYaw() { return m_yaw; }

private:
    // ī�޶��� (������Ʈ��) ���Ϸ� �������� ���� ���͸� ���
    void updateCameraVectors();
};