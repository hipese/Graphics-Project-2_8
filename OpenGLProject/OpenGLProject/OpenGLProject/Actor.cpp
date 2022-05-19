#include "Actor.h"
#include "Shader.h"
#include "Animation.h"
#include "Model.h"

Actor::Actor()
{
}

Actor::~Actor()
{
}

int Actor::GetAnimationType()
{
    return m_animationType;
}

void Actor::SetAnimationType(int animationType)
{
    m_animations[animationType]->Stop();
    m_animationType = animationType;
}

void Actor::Draw(Shader& shader)
{
    m_animations[m_animationType]->Draw(shader);
}

void Actor::Update()
{
    m_animations[m_animationType]->Update();
}