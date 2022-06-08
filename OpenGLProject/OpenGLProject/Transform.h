#pragma once
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
protected:
	Transform* m_parent = nullptr;
	glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_rotation{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };
	const glm::vec3 m_up = { 0.0f, 1.0f, 0.0f };

public:
	inline Transform* GetParent() { return m_parent; }
	inline void SetParent(Transform* parent) { m_parent = parent; }
	inline glm::vec3 GetPosition() const { return m_position; }
	inline void SetPosition(const glm::vec3& position) { m_position = position; }
	inline void AddPosition(const glm::vec3& position) { m_position += position; }
	inline glm::vec3 GetRotation() const { return m_rotation; }
	inline void SetRotation(const glm::vec3& rotation) { m_rotation = rotation; }
	inline void AddRotation(const glm::vec3& rotation) { m_rotation += rotation; }
	inline glm::vec3 GetScale() const { return m_scale; }
	inline void SetScale(const glm::vec3& scale) { m_scale = scale; }

public:
	Transform();
	virtual ~Transform();

	// Transform을 부모 Transform에 붙입니다.
	
	// Transform으로 변환된 Mat4를 반환합니다.
	glm::mat4 GetMat4();
	glm::vec3 GetFront(glm::vec3 rotation);
	glm::vec3 GetFront();
	glm::vec3 GetRight();
	glm::vec3 GetUp();
	glm::vec3 GetWorldPosition();
private:
	void GetMat4Node(Transform* next, glm::vec3& outPosition, glm::vec3& outRotation, glm::vec3& outScale);
};

