#pragma once
#include "Model.h"

class AttackModel : public Model
{
private:
	bool m_bSiege = false;
	float m_deltaTime = 0.0f;
	float m_velocity = 20.0f;
	float m_maxRange = 20.0f;
	float m_range = 0.0f;
	float m_gravity = 0.008f;
	float m_gravityVelocity = 0.0f;
	glm::vec3 m_parentFront{};
public:
	inline void SetDeltaTime(float deltaTime) { m_deltaTime = deltaTime; }
public:
	AttackModel(std::string const& path, bool gamma = false, Transform* parent = nullptr);

	virtual void Draw(class Shader& shader) override;
	void TankAttack();
	void SiegeTankAttack();

private:
	void Run();
};