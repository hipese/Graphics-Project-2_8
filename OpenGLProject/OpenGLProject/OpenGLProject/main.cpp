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

// ������ ������ ���� �̺�Ʈ ó��
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// ���콺 �ݹ� �̺�Ʈ ó��
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// ���콺 �� ��ũ�� �̺�Ʈ ó��
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// �Է� �̺�Ʈ ó��
void processInput(GLFWwindow* window);
// ������ �������� ���� �ð����� �ѹ� �� ȣ���Ͽ� ���
void Update();

// ȭ�� ����
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// ī�޶� ����
Camera camera(glm::vec3(0.0f, 0.5f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Ÿ�̸�
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// ������
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// �÷��̾�
SiegeTank* player = nullptr;
bool modeChanged = false;

int main()
{
    // glfw �ʱ�ȭ
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // mac �ü������ �߰���
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // window ��ü�� ����
    GLFWwindow* window = glfwCreateWindow(1600, 900, "OpenGLProject", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // TODO: �ݹ� �Լ� ����ϼ���
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // ���콺�� ������ ������ ������ �ʰ� ������ â�� ���� �� ������ ����(FPS ī�޶� �ý���)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // OpenGL �Լ����� ȣ���ϱ� ���� GLAD�� �ʱ�ȭ
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // stb_image.h �� ���� �ε�� �ؽ����� y���� �������� ����(�� �ε� ����)
    stbi_set_flip_vertically_on_load(true);

    // ���� OpenGL ���� ����
    glEnable(GL_DEPTH_TEST);

    // TODO: Shader�� �������ϼ���
    Shader ourShader("model_loading.vs", "model_loading.fs");

    // TODO: ���� �ε��ϼ���
    SiegeTank tank;    
    player = &tank;

    // ī�޶� ���󰡰� �մϴ�.
    // camera.SetParent(&body);

    // ���̾����������� ������
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // TODO: ������ ������ �ۼ��ϼ���
    float updateFrame = 0.0f;
    float maxUpdateFrame = 1.0f / 60.0f;
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time ����
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // ���� �ð� �ڸ��� Update ȣ��
        updateFrame += deltaTime;
        if (updateFrame >= maxUpdateFrame)
        {
            updateFrame -= maxUpdateFrame;
            Update();
        }

        // �Է� �̺�Ʈ
        processInput(window);

        // �ʱ� ������
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Shader Ȱ��ȭ
        ourShader.use();

        // view/projection ��ȯ
        glm::mat4 projection = glm::perspective(glm::radians(camera.m_zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // TODO: �� ������
        tank.Draw(ourShader);

        // glfw: ���� ��ȯ �� ����� �̺�Ʈ ����(Ű ����/Ǯ��, ���콺 �̵� ��)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ��� �ڿ� ���� �� ����
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // ����Ʈ �ڵ� ����
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    // ���� esc Ű�� ���� ������ �ִ��� Ȯ��
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
    float yoffset = lastY - ypos; // y ��ǥ�� �Ʒ����� ���� �̵��ϱ� ������ ������

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