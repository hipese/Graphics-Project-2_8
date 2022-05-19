#include "SiegeTank.h"
#include "Animation.h"

#include <iostream>
#include <string>

SiegeTank::SiegeTank()
{
    // Transform 설정
    SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    SetScale(glm::vec3(0.15f, 0.15f, 0.15f));

    //모드
    t_mode = (int)ModeType::NOT_SIEGE;

    // Animation Type 설정
    m_max_animation_type = 5;
    m_animationType = (int)EAnimationType::TANK_STAND;

    // Animation 설정
    m_animations = new Animation*[m_max_animation_type];
    for (int i = 0; i < m_max_animation_type; i++)
    {
        m_animations[i] = new Animation();
    }

    int target_max_frame = 1;
    Animation* target_animnation = m_animations[(int)EAnimationType::TANK_STAND];
    target_animnation->SetbLoop(true);
    target_animnation->LoadModel("resources/SiegeTank/Tank_Stand_", target_max_frame, this);

    target_max_frame = 36;
    target_animnation = m_animations[(int)EAnimationType::TANK_STAND_START];
    target_animnation->SetbLoop(false);
    target_animnation->LoadModel("resources/SiegeTank/Tank_Stand_Start_", target_max_frame, this);

    target_max_frame = 30;
    target_animnation = m_animations[(int)EAnimationType::TANK_WALK];
    target_animnation->SetbLoop(true);
    target_animnation->LoadModel("resources/SiegeTank/Tank_Walk_", target_max_frame, this);

    target_max_frame = 35;
    target_animnation = m_animations[(int)EAnimationType::TANK_WALK_START];
    target_animnation->SetbLoop(false);
    target_animnation->LoadModel("resources/SiegeTank/Tank_Walk_Start_", target_max_frame, this);

    target_max_frame = 30;
    target_animnation = m_animations[(int)EAnimationType::TANK_ATTACK];
    target_animnation->SetbLoop(false);
    target_animnation->LoadModel("resources/SiegeTank/Tank_Attack_", target_max_frame, this);

}

SiegeTank::~SiegeTank()
{
    for (int i = m_max_animation_type - 1; i >= 0; i--)
    {
        delete m_animations[i];
    }
    delete[] m_animations;
}

void SiegeTank::Update()
{
    m_animations[m_animationType]->Update();

    std::cout << "animType : " << m_animationType << std::endl;

}
int SiegeTank::getMode()
{
    return t_mode;
}
void SiegeTank::setMode(int mode) {
    t_mode = mode;
}