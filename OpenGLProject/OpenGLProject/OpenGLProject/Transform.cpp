#include "Transform.h"

#include <iostream>

Transform::Transform()
{
}

Transform::~Transform()
{
}

void Transform::SetParent(Transform* parent)
{
    m_parent = parent;
}

glm::mat4 Transform::GetMat4()
{
    // parent의 transfrom 불러오기
    glm::vec3 parentPosition{};
    glm::vec3 parentRotation{};
    glm::vec3 parentScale{};
    if (m_parent != nullptr)
    {
        parentPosition = m_parent->GetPosition();
        parentRotation = m_parent->GetRotation();
        parentScale = m_parent->GetScale();
    }

    glm::mat4 mat4 = glm::mat4(1.0f);

    // scene 에서의 위치 변환
    mat4 = glm::translate(mat4, parentPosition + m_position);

    // rotation 변환
    mat4 = glm::rotate(mat4, glm::radians(parentRotation.x + m_rotation).x, glm::vec3(1.0f, 0.0f, 0.0f));
    mat4 = glm::rotate(mat4, glm::radians(parentRotation.y + m_rotation).y, glm::vec3(0.0f, 1.0f, 0.0f));
    mat4 = glm::rotate(mat4, glm::radians(parentRotation.z + m_rotation).z, glm::vec3(0.0f, 0.0f, 1.0f));

    // scale 변환
    mat4 = glm::scale(mat4, parentScale + m_scale);

    return mat4;
}

void Transform::print()
{
    std::cout << "parent : " << m_parent << std::endl;
    std::cout << "position : "
        << GetPosition().x << ", "
        << GetPosition().y << ", "
        << GetPosition().z << std::endl;

    std::cout << "rotation : "
        << GetRotation().x << ", "
        << GetRotation().y << ", "
        << GetRotation().z << std::endl;

    std::cout << "scale : "
        << GetScale().x << ", "
        << GetScale().y << ", "
        << GetScale().z << std::endl;
}
