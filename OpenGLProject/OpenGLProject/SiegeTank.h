#pragma once
#include "Animator.h"
#include "Transform.h"

class SiegeTank : public Animator
{
public:
	enum class EState
	{
		TANK_STAND,
		TANK_STAND_START,
		TANK_WALK,
		TANK_WALK_START,
		TANK_ATTACK,
		TANK_MORPH,
		SIEGETANK_STAND,
		SIEGETANK_ATTACK
	};
private:
	glm::vec3 m_finalWorldTurretRotate = { 0.0f, 1.0f, 0.0f };
	glm::vec3 m_worldTurretRotate = { 0.0f, 1.0f, 0.0f };
	float m_deltaTime = 0.0f;
	float m_maxTurretPitch = 15.0f;
	class Camera* m_camera = nullptr;
	const glm::vec3 m_rightRotate{ 0.0f, 1.0f, 0.0f };
	float m_velocity = 1.0f;
	float m_rotateSpeed = 90.0f;
	bool m_bCanMove = true;
	bool m_bCanAttack = true;
	class AttackModel* m_attackModel = nullptr;

public:
	SiegeTank();
	virtual ~SiegeTank();
	virtual void Draw(class Shader& shader) override;
	virtual void Update() override;
	glm::vec3 GetTurretRotation();
	class AttackModel* GetAttackModel();
	void SetAttackModel(class AttackModel* attack);
	void SetCamera(class Camera* camera);
	void SetTurretRotation(glm::vec3 rotation);
	void SetDeltaTime(float deltaTime);
	glm::mat4 GetViewFromFront(glm::vec3 front);
	void MoveFront();
	void MoveBack();
	void RotateLeft();
	void RotateRight();
	void RotateWorldTurret(float pitch, float yaw);
	void RotateTurretHeight(float yoffset, float deltaTime);
	void Attack();
	void Morph();
	void Stand();
	void EndPlay();
	class Mesh* GetTurretMesh();

private:
	// 궤도 형식처럼 텍스처를 매핑힙니다.
	void SetCaterpillarMapping(Animation* targetAnimation, int targetMaxFrame);

	class Mesh* GetRightCaterpillar();
	class Mesh* GetLeftCaterpillar();
};

