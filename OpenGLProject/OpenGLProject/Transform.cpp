#include "Transform.h"

#include <iostream>

Transform::Transform()
{
}

Transform::~Transform()
{
}

glm::mat4 Transform::GetMat4()
{
    // parent의 transfrom 불러오기
    glm::vec3 outPosition{};
    glm::vec3 outRotation{};
    glm::vec3 outScale{};
    GetMat4Node(this, outPosition, outRotation, outScale);

    glm::mat4 mat4 = glm::mat4(1.0f);

    // scene 에서의 위치 변환
    mat4 = glm::translate(mat4, outPosition);

    // rotation 변환
    mat4 = glm::rotate(mat4, glm::radians(outRotation).x, glm::vec3(1.0f, 1.0f, 1.0f));
    mat4 = glm::rotate(mat4, glm::radians(outRotation).y, glm::vec3(0.0f, 1.0f, 0.0f));
    mat4 = glm::rotate(mat4, glm::radians(outRotation).z, glm::vec3(0.0f, 0.0f, 1.0f));

    // scale 변환
    mat4 = glm::scale(mat4, outScale);

    return mat4;
}

void Transform::GetMat4Node(Transform* next, glm::vec3& outPosition, glm::vec3& outRotation, glm::vec3& outScale)
{
    if (next->m_parent != nullptr)
    {
        GetMat4Node(next->m_parent, outPosition, outRotation, outScale);
        outPosition += next->GetPosition();
        outRotation += next->GetRotation();
        outScale *= next->GetScale();
    }
    else
    {
        outPosition = next->m_position;
        outRotation = next->m_rotation;
        outScale = next->m_scale;
    }
}


glm::vec3 Transform::GetFront(glm::vec3 rotation)
{
    float pitch = rotation.x;
    float yaw = rotation.y;
    glm::vec3 front{};

    front.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    return front;
}

glm::vec3 Transform::GetFront()
{
    // parent의 transfrom 불러오기
    glm::vec3 outPosition{};
    glm::vec3 outRotation{};
    glm::vec3 outScale{};
    GetMat4Node(this, outPosition, outRotation, outScale);

    return GetFront(outRotation);
}

glm::vec3 Transform::GetRight()
{
    return glm::normalize(glm::cross(GetFront(), GetUp()));
}

glm::vec3 Transform::GetUp()
{
    return m_up;
}

glm::vec3 Transform::GetWorldPosition()
{
    // parent의 transfrom 불러오기
    glm::vec3 outPosition{};
    glm::vec3 outRotation{};
    glm::vec3 outScale{};
    GetMat4Node(this, outPosition, outRotation, outScale);

    return outPosition;
}
