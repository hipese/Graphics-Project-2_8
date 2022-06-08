#pragma once
#include "Transform.h"

// 카메라 이동에 대한 몇 가지 가능한 옵션을 정의
enum class Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// 카메라의 기본 변수
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 30.0f;

// 입력을 처리하고 OpenGL에서 사용할 해당 오일러 각도, 벡터 및 행렬을 계산하는 카메라 클래스
class Camera : public Transform
{
public:
    // 카메라 속성
    glm::vec3 m_front{};
    glm::vec3 m_up{};
    glm::vec3 m_right{};
    glm::vec3 m_worldUp{};
    // 오일러 각도
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;
    // 카메라 설정
    float m_movementSpeed = 0.0f;
    float m_mouseSensitivity = 0.0f;
    float m_zoom = 0.0f;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    
    // 오일러 각도 및 LookAt 행렬를 사용하여 계산된 뷰 행렬을 반환
    glm::mat4 GetViewMatrix();
    // 마우스 입력 시스템에서 받은 입력을 처리
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    // 마우스 휠 스크롤 이벤트
    void ProcessMouseScroll(float yoffset);
    // 렌더링 루프 업데이트
    void OnUpdate();
    // 전면 벡터 반환
    inline glm::vec3 GetFront(){return m_front;}
    inline float GetYaw() { return m_yaw; }

private:
    // 카메라의 (업데이트된) 오일러 각도에서 전면 벡터를 계산
    void updateCameraVectors();
};