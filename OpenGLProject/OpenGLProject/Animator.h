#pragma once
#include "Transform.h"

class Animator : public Transform
{
protected:
	class Animation** m_animations = nullptr;
	int m_maxAnimationType = 0;
	int m_animationType = 0;
public:
	inline int GetAnimationType() { return m_animationType; };
	inline class Animation* GetAnimation() { return m_animations[m_animationType]; }


public:
	Animator();
	virtual ~Animator();

	class Model* GetModel();
	void SetAnimationType(int animationType);
	virtual void Draw(class Shader& shader);
	virtual void Update();
};

