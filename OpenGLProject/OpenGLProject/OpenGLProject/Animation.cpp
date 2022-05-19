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

    for (int i = m_max_frame - 1; i >= 0; i--)
    {
        delete m_models[i];
    }
    delete[] m_models;
}

void Animation::LoadModel(const char* files, int max_frame, Transform* parent)
{
    m_max_frame = max_frame;

    m_models = new Model* [max_frame];
    for (int i = 0; i < max_frame; i++)
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

void Animation::InitFrame()
{
    m_current_frame = 0;
}

void Animation::PrevFrame()
{
    if (m_current_frame > 0)
    {
        m_current_frame--;
        if (m_current_frame < 0 && m_bLoop)
        {
            m_current_frame = m_max_frame - m_current_frame;
        }
    }
}

void Animation::NextFrame()
{
    if (m_current_frame < m_max_frame)
    {
        m_current_frame++;
        if (m_current_frame >= m_max_frame && m_bLoop)
        {
            m_current_frame = m_current_frame - m_max_frame;
        }
    }
}

void Animation::Draw(Shader& shader)
{
    m_models[m_current_frame]->Draw(shader);
}

void Animation::Update()
{
    if (m_Playing == 1)
    {
        NextFrame();
    }
    else if (m_Playing == -1)
    {
        PrevFrame();
    }
}
