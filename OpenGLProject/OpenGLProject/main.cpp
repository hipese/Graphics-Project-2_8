#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Animator.h"
#include "Model.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Animation.h"
#include "SiegeTank.h"
#include "AttackModel.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// ������ ������ ���� �̺�Ʈ ó��
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// ���콺 �ݹ� �̺�Ʈ ó��
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// ���콺 �� ��ũ�� �̺�Ʈ ó��
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
// �Է� �̺�Ʈ ó��
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
// ������ �������� ���� �ð����� �ѹ� �� ȣ���Ͽ� ���
void Update();

// ȭ�� ����
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

// ī�޶� ����
Camera camera(glm::vec3(0.0f, 0.3f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Ÿ�̸�
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// �÷��̾�
SiegeTank* player = nullptr;

// vao
unsigned int shadowVAO;
unsigned int planeVAO;

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGLProject", NULL, NULL);
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
    glfwSetMouseButtonCallback(window, mouse_button_callback);
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // TODO: Shader�� �������ϼ���
    Shader shader("model_loading.vs", "model_loading.fs");

    // TODO: ���� �ε��ϼ���
    SiegeTank tank;
    player = &tank;
    player->SetRotation(glm::vec3(0.0f, 180.0f, 0.0f));
    AttackModel attack("resources/bullet.obj", 0, tank.GetTurretMesh());
    attack.m_visible = false;
    tank.SetAttackModel(&attack);

    // ī�޶� ���󰡰� �մϴ�.
    camera.SetParent(&tank);
    player->SetCamera(&camera);

    // ���̾����������� ������
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Shadow vertex
    float shadowVertices[] = {
        // positions            // normals         // texcoords
         1.6f, 0.01f,  2.2f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -1.6f, 0.01f,  2.2f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
        -1.6f, 0.01f, -2.2f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
         
         1.6f, 0.01f,  2.2f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -1.6f, 0.01f, -2.2f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
         1.6f, 0.01f, -2.2f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f
    };

    // Plane vertex
    float planeVertices[] = {
        // positions            // normals         // texcoords
         25.0f, 0.0f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, 0.0f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, 0.0f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         
         25.0f, 0.0f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, 0.0f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, 0.0f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };

    // Shadow VAO
    unsigned int shadowVBO;
    glGenVertexArrays(1, &shadowVAO);
    glGenBuffers(1, &shadowVBO);
    glBindVertexArray(shadowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, shadowVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shadowVertices), shadowVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    // plane VAO
    unsigned int planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    // load textures
    unsigned int floorTexture = loadTexture("resources/textures/marble.jpg");
    unsigned int shadowTexture = loadTexture("resources/textures/shadow.png");

    // shader configuration
    shader.use();
    shader.setInt("diffuseMap", 0);
    shader.setInt("normalMap", 1);
    shader.setBool("alpha", false);

    // lighting info
    glm::vec3 lightPos(50.0f, 100.0f, 50.0f);

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
        glClearColor((102.0f /255.0f), 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Camera Position ����
        glm::vec3 cameraPos = camera.GetPosition();
        glm::vec3 cameraNewPos = camera.GetPosition();
        float backMultiple = 2.0f;
        cameraNewPos += -camera.GetFront() * backMultiple;
        camera.SetPosition(cameraNewPos);

        // view/projection ��ȯ
        glm::mat4 projection = glm::perspective(glm::radians(camera.m_zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // Shader Ȱ��ȭ
        shader.use();

        // TODO: �� ������
        // floor
        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        shader.setBool("alpha", false);
        glBindVertexArray(planeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // shadow
        model = glm::mat4(1.0f);
        model = glm::translate(model, tank.GetPosition());
        model = glm::rotate(model, glm::radians(tank.GetRotation()).x, glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::rotate(model, glm::radians(tank.GetRotation()).y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(tank.GetRotation()).z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, tank.GetScale());
        shader.setMat4("model", model);
        shader.setBool("alpha", true);
        glBindVertexArray(shadowVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadowTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // tank
        shader.setBool("alpha", false);
        shader.setVec3("viewPos", camera.GetPosition());
        shader.setVec3("lightPos", lightPos);
        tank.Draw(shader);
        attack.Draw(shader);

        // glfw: ���� ��ȯ �� ����� �̺�Ʈ ����(Ű ����/Ǯ��, ���콺 �̵� ��)
        camera.SetPosition(cameraPos);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteVertexArrays(1, &shadowVAO);
    glDeleteBuffers(1, &shadowVBO);

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

    if (!player)
    {
        return;
    }

    player->SetDeltaTime(deltaTime);

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        player->Morph();
    }

    bool bIsMove = false;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        player->MoveFront();
        bIsMove = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        player->MoveBack();
        bIsMove = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        player->RotateLeft();
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        player->RotateRight();
    }
    if (!bIsMove)
    {
        player->Stand();
    }
    player->EndPlay();
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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        player->Attack();
    }

}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //player->RotateTurretHeight(static_cast<float>(yoffset), deltaTime);
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void Update()
{
    player->Update();
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}