#include "SiegeTank.h"
#include "Animation.h"
#include "Camera.h"
#include "AttackModel.h"

#include <iostream>
#include <string>

SiegeTank::SiegeTank()
{
    // Transform 설정
    SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    SetScale(glm::vec3(0.15f, 0.15f, 0.15f));

    // Animation Type 설정
    m_maxAnimationType = 8;
    m_animationType = (int)EState::TANK_STAND;

    // Animation 설정
    m_animations = new Animation*[m_maxAnimationType];
    for (int i = 0; i < m_maxAnimationType; i++)
    {
        m_animations[i] = new Animation();
    }

    Animation* targetAnimnation = m_animations[(int)EState::TANK_STAND];
    targetAnimnation->LoadModel("resources/SiegeTank/Tank_Stand_", 1, this);

    targetAnimnation = m_animations[(int)EState::TANK_STAND_START];
    targetAnimnation->LoadModel("resources/SiegeTank/Tank_Stand_Start_", 36, this);

    targetAnimnation = m_animations[(int)EState::TANK_WALK];
    targetAnimnation->SetbLoop(true);
    targetAnimnation->LoadModel("resources/SiegeTank/Tank_Walk_", 30, this);
    SetCaterpillarMapping(targetAnimnation, 30);

    targetAnimnation = m_animations[(int)EState::TANK_WALK_START];
    targetAnimnation->LoadModel("resources/SiegeTank/Tank_Walk_Start_", 35, this);
    SetCaterpillarMapping(targetAnimnation, 35);

    targetAnimnation = m_animations[(int)EState::TANK_ATTACK];
    targetAnimnation->LoadModel("resources/SiegeTank/Tank_Attack_", 30, this);
    targetAnimnation->SetDelay(30);

    targetAnimnation = m_animations[(int)EState::TANK_MORPH];
    targetAnimnation->LoadModel("resources/SiegeTank/Tank_Morph_", 110, this);

    targetAnimnation = m_animations[(int)EState::SIEGETANK_STAND];
    targetAnimnation->LoadModel("resources/SiegeTank/SiegeTank_Stand_", 1, this);

    targetAnimnation = m_animations[(int)EState::SIEGETANK_ATTACK];
    targetAnimnation->LoadModel("resources/SiegeTank/SiegeTank_Attack_", 30, this);
    targetAnimnation->SetDelay(120);
    
}

SiegeTank::~SiegeTank()
{
    for (int i = m_maxAnimationType - 1; i >= 0; i--)
    {
        delete m_animations[i];
    }
    delete[] m_animations;
}

void SiegeTank::Draw(Shader& shader)
{
    RotateWorldTurret(-m_camera->m_pitch, -m_camera->m_yaw + 90.0f);
    SetTurretRotation(m_worldTurretRotate - GetRotation());
    Animator::Draw(shader);
}

void SiegeTank::Update()
{
    m_animations[m_animationType]->Update();
}

glm::vec3 SiegeTank::GetTurretRotation()
{
    return GetTurretMesh()->GetRotation();
}

AttackModel* SiegeTank::GetAttackModel()
{
    return m_attackModel;
}

void SiegeTank::SetAttackModel(AttackModel* attack)
{
    m_attackModel = attack;
}

void SiegeTank::SetCamera(Camera* camera)
{
    m_camera = camera;
    m_finalWorldTurretRotate = glm::vec3(0.0f, -m_camera->m_yaw + 90.0f, 0.0f);
    m_worldTurretRotate += m_finalWorldTurretRotate;
}

void SiegeTank::SetTurretRotation(glm::vec3 rotation)
{
    GetTurretMesh()->SetRotation(rotation);
}

void SiegeTank::SetDeltaTime(float deltaTime)
{
    m_deltaTime = deltaTime;
    GetAttackModel()->SetDeltaTime(deltaTime);
}

glm::mat4 SiegeTank::GetViewFromFront(glm::vec3 front)
{
    glm::vec3 const pos = GetPosition();
    glm::vec3 const target = pos + front;
    glm::vec3 const up(0.0f, 1.0f, 0.0f);
    
    return glm::lookAt(GetPosition(), target, up);
}

void SiegeTank::MoveFront()
{
    if (!m_bCanMove)
    {
        return;
    }

    float velocity = m_velocity * m_deltaTime;
    AddPosition(GetFront() * velocity);
    if (GetAnimationType() != (int)SiegeTank::EState::TANK_WALK_START)
    {
        if (GetAnimationType() == (int)SiegeTank::EState::TANK_WALK)
        {
            return;
        }
        SetAnimationType((int)SiegeTank::EState::TANK_WALK_START);
        GetAnimation()->Play();
    }
    else if (GetAnimation()->IsEndPlay())
    {
        SetAnimationType((int)SiegeTank::EState::TANK_WALK);
        GetAnimation()->Play();
    }
}

void SiegeTank::MoveBack()
{
    if (!m_bCanMove)
    {
        return;
    }

    float velocity = m_velocity * m_deltaTime * 0.5f;
    AddPosition(GetFront() * -velocity);
    if (GetAnimationType() != (int)SiegeTank::EState::TANK_WALK)
    {
        SetAnimationType((int)SiegeTank::EState::TANK_WALK);
        GetAnimation()->ReversePlay();
    }
}

void SiegeTank::RotateLeft()
{
    if (!m_bCanMove)
    {
        return;
    }

    float rotateSpeed = 90.0f * m_deltaTime;
    AddRotation(m_rightRotate * rotateSpeed);
}

void SiegeTank::RotateRight()
{
    if (!m_bCanMove)
    {
        return;
    }

    float rotateSpeed = 90.0f * m_deltaTime;
    AddRotation(m_rightRotate * -rotateSpeed); 
}

void SiegeTank::RotateWorldTurret(float pitch, float yaw)
{
    if (pitch > m_maxTurretPitch)
    {
        pitch = m_maxTurretPitch;
    }
    else if (pitch < 0.0f)
    {
        pitch = 0.0f;
    }
    m_finalWorldTurretRotate = glm::vec3(0.0f, yaw, 0.0f);
    m_worldTurretRotate += (m_finalWorldTurretRotate - m_worldTurretRotate) * m_deltaTime;
}

void SiegeTank::RotateTurretHeight(float yoffset, float deltaTime)
{
    //std::cout << "turretVec : " << m_turretRotate.x << ", " << m_turretRotate.z << ", ";
    //std::cout << GetTurretMesh()->GetPosition().x << ", " << GetTurretMesh()->GetPosition().y << ", " << GetTurretMesh()->GetPosition().z << std::endl;
    //GetTurretMesh()->SetRotation(glm::vec3{ 0.0f ,0.0f,0.0f });
    //GetTurretMesh()->AddRotation(glm::vec3{ 0.0f ,0.0f,-45.0f });
    //GetTurretMesh()->AddRotation(glm::vec3{ 0.0f ,-45.0f,0.0f });
    float rotateSpeed = 20.0f  * yoffset;

    glm::vec3 rightVec = { 0.05f,-0.03f,-0.04f };
    glm::normalize(rightVec);
    GetTurretMesh()->AddRotation(glm::vec3{ rightVec * rotateSpeed });

    
    //GetTurretMesh()->AddRotation(glm::vec3{ 30.0f * yoffset,0.0f,0.0f });
    //glm::normalize(glm::cross(GetFront(), GetUp()));
    //GetTurretMesh()->SetPosition(glm::vec3{ 0.0f,-0.1f,0.0f });
    //AddRotation(rightVec* rotateSpeed);
    //GetTurretMesh()->AddRotation(m_turretRotate * rotateSpeed);

}

void SiegeTank::Attack()
{
    if (!m_bCanAttack)
    {
        return;
    }
    m_bCanMove = false;
    if (GetAnimationType() == (int)SiegeTank::EState::TANK_STAND ||
        GetAnimationType() == (int)SiegeTank::EState::TANK_STAND_START || 
        GetAnimationType() == (int)SiegeTank::EState::TANK_WALK || 
        GetAnimationType() == (int)SiegeTank::EState::TANK_WALK_START)
    {
        if (GetAnimationType() == (int)SiegeTank::EState::TANK_ATTACK)
        {
            return;
        }
        SetAnimationType((int)SiegeTank::EState::TANK_ATTACK);
        GetAnimation()->Play();
        GetAttackModel()->SetParent(GetTurretMesh());
        SetTurretRotation(m_worldTurretRotate - GetRotation());
        GetAttackModel()->TankAttack();
    }
    else if(GetAnimationType() == (int)SiegeTank::EState::SIEGETANK_STAND)
    {
        if (GetAnimationType() == (int)SiegeTank::EState::SIEGETANK_ATTACK)
        {
            return;
        }
        SetAnimationType((int)SiegeTank::EState::SIEGETANK_ATTACK);
        GetAnimation()->Play();
        GetAttackModel()->SetParent(GetTurretMesh());
        SetTurretRotation(m_worldTurretRotate - GetRotation());
        std::cout << m_worldTurretRotate.y << std::endl;
        GetAttackModel()->SiegeTankAttack();
    }
}

void SiegeTank::Morph()
{
    if (GetAnimationType() == (int)SiegeTank::EState::TANK_MORPH)
    {
        return;
    }

    if (GetAnimationType() == (int)SiegeTank::EState::TANK_STAND)
    {
        m_bCanMove = false;
        m_bCanAttack = false;
        SetAnimationType((int)SiegeTank::EState::TANK_MORPH);
        GetAnimation()->Play();
    }
    else if (GetAnimationType() == (int)SiegeTank::EState::SIEGETANK_STAND)
    {
        m_bCanMove = false;
        m_bCanAttack = false;
        SetAnimationType((int)SiegeTank::EState::TANK_MORPH);
        m_worldTurretRotate.y += 180.0f;
        GetAnimation()->ReversePlay();
    }
}

void SiegeTank::Stand()
{
    if (GetAnimationType() == (int)SiegeTank::EState::TANK_WALK_START ||
        GetAnimationType() == (int)SiegeTank::EState::TANK_WALK &&
        GetAnimationType() != (int)SiegeTank::EState::TANK_STAND_START)
    {
        SetAnimationType((int)SiegeTank::EState::TANK_STAND_START);
        if (GetAnimation()->IsReverse())
        {
            GetAnimation()->ReversePlay();
        }
        else
        {
            GetAnimation()->Play();
        }
    }
    
}

void SiegeTank::EndPlay()
{
    if (!GetAnimation()->IsEndPlay() || !GetAnimation()->IsPlaying())
    {
        return;
    }

    if (GetAnimationType() == (int)SiegeTank::EState::TANK_STAND_START)
    {
        SetAnimationType((int)SiegeTank::EState::TANK_STAND);
    }
    else if (GetAnimationType() == (int)SiegeTank::EState::TANK_ATTACK)
    {
        m_bCanMove = true;
        SetAnimationType((int)SiegeTank::EState::TANK_STAND);
    }
    else if (GetAnimationType() == (int)SiegeTank::EState::SIEGETANK_ATTACK)
    {
        SetAnimationType((int)SiegeTank::EState::SIEGETANK_STAND);
    }
    else if (GetAnimationType() == (int)SiegeTank::EState::TANK_MORPH)
    {
        if (GetAnimation()->IsReverse())
        {
            m_bCanMove = true;
            m_bCanAttack = true;
            SetAnimationType((int)SiegeTank::EState::TANK_STAND);
        }
        else
        {
            m_bCanAttack = true;
            SetAnimationType((int)SiegeTank::EState::SIEGETANK_STAND);
            m_worldTurretRotate.y -= 180.0f;
        }
    }
}

void SiegeTank::SetCaterpillarMapping(Animation* targetAnimation, int targetMaxFrame)
{
    Model** walkModels = targetAnimation->GetModels();
    for (int i = 0; i < targetMaxFrame; i++)
    {
        Model* walkModel = walkModels[i];
        for (Vertex& vertex : walkModel->m_meshes[0].vertices)
        {
            vertex.TexCoords.y -= 1.0f / targetMaxFrame * i;

        }
        for (Vertex& vertex : walkModel->m_meshes[2].vertices)
        {
            vertex.TexCoords.y -= 1.0f / targetMaxFrame * i;

        }
        walkModel->m_meshes[0].setupMesh();
        walkModel->m_meshes[2].setupMesh();
    }
}

Mesh* SiegeTank::GetTurretMesh()
{
    return &GetAnimation()->GetModel()->m_meshes[4];
}
Mesh* SiegeTank::GetRightCaterpillar()
{
    return &GetAnimation()->GetModel()->m_meshes[0];
}
Mesh* SiegeTank::GetLeftCaterpillar()
{
    return &GetAnimation()->GetModel()->m_meshes[2];
}