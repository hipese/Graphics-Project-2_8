#pragma once
#include "Transform.h"

class Actor : public Transform
{
protected:
	class Animation** m_animations = nullptr;
	int m_max_animation_type = 0;
	int m_animationType = 0;

public:
	Actor();
	virtual ~Actor();

	class Animation* GetAnimation() { return m_animations[m_animationType]; }
	int GetAnimationType();
	void SetAnimationType(int animationType);
	// Actor를 그립니다.
	virtual void Draw(class Shader& shader);
	virtual void Update();
};

