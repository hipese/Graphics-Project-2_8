#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Actor.h"
#include "Model.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Animation.h"
#include "SiegeTank.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// 윈도우 사이즈 변경 이벤트 처리
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 마우스 콜백 이벤트 처리
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// 마우스 휠 스크롤 이벤트 처리
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// 입력 이벤트 처리
void processInput(GLFWwindow* window);
// 렌더링 루프에서 일정 시간마다 한번 씩 호출하여 사용
void Update();

// 화면 설정
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 카메라 설정
Camera camera(glm::vec3(0.0f, 0.5f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// 타이머
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 라이팅
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// 플레이어
SiegeTank* player = nullptr;
bool modeChanged = false;

int main()
{
    // glfw 초기화
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // mac 운영체제에서 추가됨
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // window 객체를 생성
    GLFWwindow* window = glfwCreateWindow(1600, 900, "OpenGLProject", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // TODO: 콜백 함수 등록하세요
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // 마우스를 움직일 때마다 보이지 않고 윈도우 창을 떠날 수 없도록 설정(FPS 카메라 시스템)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // OpenGL 함수들을 호출하기 전에 GLAD를 초기화
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // stb_image.h 를 통해 로드된 텍스쳐의 y축을 뒤집도록 설정(모델 로드 전에)
    stbi_set_flip_vertically_on_load(true);

    // 전역 OpenGL 상태 설정
    glEnable(GL_DEPTH_TEST);

    // TODO: Shader를 컴파일하세요
    Shader ourShader("model_loading.vs", "model_loading.fs");

    // TODO: 모델을 로드하세요
    SiegeTank tank;    
    player = &tank;

    // 카메라가 따라가게 합니다.
    // camera.SetParent(&body);

    // 와이어프레임으로 렌더링
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // TODO: 렌더링 루프를 작성하세요
    float updateFrame = 0.0f;
    float maxUpdateFrame = 1.0f / 60.0f;
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time 로직
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 일정 시간 뒤마다 Update 호출
        updateFrame += deltaTime;
        if (updateFrame >= maxUpdateFrame)
        {
            updateFrame -= maxUpdateFrame;
            Update();
        }

        // 입력 이벤트
        processInput(window);

        // 초기 렌더링
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Shader 활성화
        ourShader.use();

        // view/projection 변환
        glm::mat4 projection = glm::perspective(glm::radians(camera.m_zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // TODO: 모델 렌더링
        tank.Draw(ourShader);

        // glfw: 버퍼 교환 및 입출력 이벤트 폴링(키 누름/풀림, 마우스 이동 등)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 모든 자원 정리 및 삭제
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 뷰포트 자동 설정
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    // 만약 esc 키가 현재 눌려져 있는지 확인
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float velocity = 1.0f * deltaTime;
    float pitch = player->GetRotation().x;
    float yaw = player->GetRotation().y;
    const glm::vec3 worldUp{ 0.0f, -1.0f, 0.0f };
    glm::vec3 front{};
    glm::vec3 left{};
    glm::vec3 up{};
    
    front.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    left = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(left, front));

    if (!player)
    {
        return;
    }

    bool bMove = false;
    

    if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !modeChanged)
    {
        if (player->getMode() == (int)SiegeTank::ModeType::NOT_SIEGE) {
            player->setMode((int)SiegeTank::ModeType::SIEGE);
            modeChanged = true;
        }           
        else if(player->getMode() == (int)SiegeTank::ModeType::SIEGE) {
            player->setMode((int)SiegeTank::ModeType::NOT_SIEGE);
            modeChanged = true;
        }    
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) {
        modeChanged = false;
    }
    if (player->getMode() == (int)SiegeTank::ModeType::NOT_SIEGE) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            player->AddPosition(front * velocity);
            bMove = true;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            player->AddPosition(-front * velocity);
            bMove = true;
        }

        glm::vec3 left_rotate{ 0.0f, 1.0f, 0.0f };
        float rotate_speed = 90.0f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            player->AddRotation(left_rotate * rotate_speed);
            player->AddPosition(front * velocity);
            bMove = true;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            player->AddRotation(-left_rotate * rotate_speed);
            player->AddPosition(front * velocity);
            bMove = true;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            player->AddRotation(-left_rotate * rotate_speed);
            player->AddPosition(-front * velocity);
            bMove = true;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            player->AddRotation(left_rotate * rotate_speed);
            player->AddPosition(-front * velocity);
            bMove = true;
        }
    }
    


    if (bMove)
    {
        if (player->GetAnimationType() != (int)SiegeTank::EAnimationType::TANK_WALK_START)
        {
            if (player->GetAnimationType() != (int)SiegeTank::EAnimationType::TANK_WALK)
            {
                player->SetAnimationType((int)SiegeTank::EAnimationType::TANK_WALK_START);
                player->GetAnimation()->Play();
            }
        }
        else if (player->GetAnimation()->IsEndFrame())
        {
            player->SetAnimationType((int)SiegeTank::EAnimationType::TANK_WALK);
            player->GetAnimation()->Play();
        }
    }
    else if(player->GetAnimationType() != (int)SiegeTank::EAnimationType::TANK_STAND)
    {
        if (player->GetAnimationType() != (int)SiegeTank::EAnimationType::TANK_STAND_START)
        {
            player->SetAnimationType((int)SiegeTank::EAnimationType::TANK_STAND_START);
            player->GetAnimation()->Play();
        }
        else if(player->GetAnimation()->IsEndFrame())
        {
            player->SetAnimationType((int)SiegeTank::EAnimationType::TANK_STAND);
            player->GetAnimation()->Play();
        }
    }

    
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // y 좌표가 아래에서 위로 이동하기 때문에 반전됨

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void Update()
{
    player->Update();
}