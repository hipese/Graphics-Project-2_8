#include "Model.h"
#include "Shader.h"
#include "stb_image.h"

std::vector<Texture> Model::textures_loaded{};

Model::Model(std::string const& path, bool gamma, Transform* parent)
    : m_gammaCorrection(gamma)
{
    loadModel(path);

    m_parent = parent;
}

void Model::Draw(Shader& shader)
{
    if (!m_visible)
    {
        return;
    }

    for (unsigned int i = 0; i < m_meshes.size(); i++)
    {
        glm::mat4 model = m_meshes[i].GetMat4();
        shader.setMat4("model", model);

        m_meshes[i].Draw(shader);
    }
}

void Model::loadModel(std::string const& path)
{
    // ASSIMP로 파일 읽기
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    
    // 오류 체크
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    // 파일 경로의 디렉토리 경로 검색
    m_directory = path.substr(0, path.find_last_of('/'));

    // ASSIMP의 루트 노드를 재귀적으로 처리
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // 현재 노드에 있는 각 메쉬 처리
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // 노드 객체는 장면의 실제 객체를 인덱싱하는 인덱스만 포함합니다.
        // 장면에는 모든 데이터가 포함되어 있습니다. 노드는 항목을 정리하기 위한 것입니다(예: 노드 간의 관계).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }
    // 모든 메쉬(있는 경우)를 처리한 후 각 자식 노드를 재귀적으로 처리
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // 설정할 데이터
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    // 각 메쉬 정점을 통과
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // assimp는 glm의 vec3 클래스로 직접 변환하지 않는 자체 벡터 클래스를 사용하므로 자리 표시자 벡터를 선언하므로 먼저 이 자리 표시자 glm::vec3에 데이터를 전송합니다.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // 만약 메쉬에 텍스처 좌표가 포함됬다면?
        {
            glm::vec2 vec;
            // 정점은 최대 8개의 서로 다른 텍스처 좌표를 포함할 수 있습니다. 따라서 우리는 그렇지 않을 것이라고 가정합니다.
            // 정점이 여러 텍스처 좌표를 가질 수 있는 모델을 사용하여 항상 첫 번째 세트(0)를 사용합니다.
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // 각 메쉬 면(삼각형)을 살펴보고 해당 정점 인덱스를 검색
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // face의 모든 인덱스를 검색하고 인덱스 벡터에 저장
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // 셰이더의 샘플러 이름에 대한 규칙을 가정합니다. 각 확산 텍스처의 이름을 지정해야 합니다.
    // 'texture_diffuseN'으로 여기에서 N은 1에서 MAX_SAMPLER_NUMBER까지의 일련 번호입니다.
    // 다음 목록이 요약한 것처럼 다른 텍스처에도 동일하게 적용됩니다.
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // 추출된 Mesh 데이터에서 생성된 Mesh 객체 반환
    return Mesh(vertices, indices, textures, this);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // 텍스처가 이전에 로드되었는지 확인하고 그렇다면 새 텍스처 로드를 건너뜁니다.
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // 동일한 파일 경로를 가진 텍스처가 이미 로드되었으면 스킵(최적화)
                break;
            }
        }
        if (!skip)
        {   // 텍스처가 아직 로드되지 않은 경우 로드
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->m_directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); //중복 텍스처를 불필요하게 로드하지 않도록 전체 모델에 대해 로드된 텍스처로 저장
        }
    }
    return textures;
}

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = 0;
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