#pragma once
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "Mesh.h"
#include "Transform.h"

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model : public Transform
{
public:
    // model ������
    static std::vector<Texture> textures_loaded;	// ���ݱ��� �ε�� ��� �ؽ�ó�� �����ϰ� �ؽ�ó�� �� �� �̻� �ε���� �ʵ��� ����ȭ
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    Model(std::string const& path, bool gamma = false, Transform* parent = nullptr);

    // Model ����
    virtual void Draw(class Shader& shader);

private:
    // ���Ͽ��� �����Ǵ� ASSIMP Ȯ���� �ִ� ���� �ε��ϰ� ��� �޽��� �޽� ���Ϳ� ����
    void loadModel(std::string const& path);

    // ����� ������� ��带 ó��, ��忡 �ִ� �� ���� �޽ø� ó���ϰ� �ڽ� ���(�ִ� ���)���� �� ���μ����� �ݺ�
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // �־��� ������ ��� ���� �ؽ�ó�� Ȯ���ϰ� ���� �ε���� ���� ��� �ؽ�ó�� �ε�
    // �ʿ��� ������ �ؽ�ó ����ü�� ��ȯ
    std::vector<struct Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);