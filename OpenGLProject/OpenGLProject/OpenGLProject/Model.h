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
    // model 데이터
    static std::vector<Texture> textures_loaded;	// 지금까지 로드된 모든 텍스처를 저장하고 텍스처가 두 번 이상 로드되지 않도록 최적화
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    Model(std::string const& path, bool gamma = false, Transform* parent = nullptr);

    // Model 렌더
    virtual void Draw(class Shader& shader);

private:
    // 파일에서 지원되는 ASSIMP 확장이 있는 모델을 로드하고 결과 메쉬를 메쉬 벡터에 저장
    void loadModel(std::string const& path);

    // 재귀적 방식으로 노드를 처리, 노드에 있는 각 개별 메시를 처리하고 자식 노드(있는 경우)에서 이 프로세스를 반복
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // 주어진 유형의 모든 재질 텍스처를 확인하고 아직 로드되지 않은 경우 텍스처를 로드
    // 필요한 정보는 텍스처 구조체로 반환
    std::vector<struct Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);