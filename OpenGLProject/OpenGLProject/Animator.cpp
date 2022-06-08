#include "Animator.h"
#include "Shader.h"
#include "Animation.h"
#include "Model.h"

Animator::Animator()
{
}

Animator::~Animator()
{
}

void Animator::Draw(Shader& shader)
{
    m_animations[m_animationType]->Draw(shader);
}

void Animator::Update()
{
    m_animations[m_animationType]->Update();
}

Model* Animator::GetModel()
{
    return GetAnimation()->GetModel();
}

void Animator::SetAnimationType(int animationType)
{
    m_animationType = animationType; m_animations[animationType]->Stop();
}