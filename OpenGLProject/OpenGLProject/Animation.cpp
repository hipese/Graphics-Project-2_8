#include "Animation.h"

#include <iostream>
#include <string>

#include "Shader.h"
#include "Transform.h"

Animation::Animation()
{
}

Animation::~Animation()
{
    if (m_models == nullptr)
    {
        return;
    }

    for (int i = m_maxFrame - 1; i >= 0; i--)
    {
        delete m_models[i];
    }
    delete[] m_models;
}

void Animation::LoadModel(const char* files, int maxFrame, Transform* parent)
{
    m_maxFrame = maxFrame;

    m_models = new Model* [maxFrame];
    for (int i = 0; i < maxFrame; i++)
    {
        std::string back = std::to_string(i);
        for (int j = (int)back.size(); j < 6; j++)
        {
            back.insert(back.begin(), '0');
        }

        std::string file = std::string(files) + back + std::string(".obj");
        m_models[i] = new Model(file, false, parent);
    }
}

void Animation::InitFrame(bool bIsReverse)
{
    m_bEndPlay = false;
    m_bIsReverse = bIsReverse;
    m_currentFrame = 0;
    m_currentDelay = m_delay;
    m_currentFrame = bIsReverse ? m_currentFrame = m_maxFrame - 1 : m_currentFrame = 0;
}

void Animation::PrevFrame()
{
    if (m_currentFrame > 0)
    {
        m_currentFrame--;
    }
    else if (m_currentDelay > 0)
    {
        m_currentDelay--;
    }
    else if (m_bLoop)
    {
        ReversePlay();
    }
    else
    {
        m_bEndPlay = true;
    }
}

void Animation::NextFrame()
{
    if (m_currentFrame < m_maxFrame - 1)
    {
        m_currentFrame++;
    }
    else if(m_currentDelay > 0)
    {
        m_currentDelay--;
    }
    else if (m_bLoop)
    {
        Play();
    }
    else
    {
        m_bEndPlay = true;
    }
}

void Animation::Draw(Shader& shader)
{
    m_models[m_currentFrame]->Draw(shader);
}

void Animation::Update()
{
    if (m_playing == 1)
    {
        NextFrame();
    }
    else if (m_playing == -1)
    {
        PrevFrame();
    }
    //std::cout << "current frame : " << m_currentFrame << std::endl;
    //std::cout << "current delay : " << m_currentDelay << std::endl;
}
