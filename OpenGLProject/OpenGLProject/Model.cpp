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
    // ASSIMP�� ���� �б�
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    
    // ���� üũ
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    // ���� ����� ���丮 ��� �˻�
    m_directory = path.substr(0, path.find_last_of('/'));

    // ASSIMP�� ��Ʈ ��带 ��������� ó��
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // ���� ��忡 �ִ� �� �޽� ó��
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // ��� ��ü�� ����� ���� ��ü�� �ε����ϴ� �ε����� �����մϴ�.
        // ��鿡�� ��� �����Ͱ� ���ԵǾ� �ֽ��ϴ�. ���� �׸��� �����ϱ� ���� ���Դϴ�(��: ��� ���� ����).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }
    // ��� �޽�(�ִ� ���)�� ó���� �� �� �ڽ� ��带 ��������� ó��
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // ������ ������
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    // �� �޽� ������ ���
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // assimp�� glm�� vec3 Ŭ������ ���� ��ȯ���� �ʴ� ��ü ���� Ŭ������ ����ϹǷ� �ڸ� ǥ���� ���͸� �����ϹǷ� ���� �� �ڸ� ǥ���� glm::vec3�� �����͸� �����մϴ�.
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
        if (mesh->mTextureCoords[0]) // ���� �޽��� �ؽ�ó ��ǥ�� ���ԉ�ٸ�?
        {
            glm::vec2 vec;
            // ������ �ִ� 8���� ���� �ٸ� �ؽ�ó ��ǥ�� ������ �� �ֽ��ϴ�. ���� �츮�� �׷��� ���� ���̶�� �����մϴ�.
            // ������ ���� �ؽ�ó ��ǥ�� ���� �� �ִ� ���� ����Ͽ� �׻� ù ��° ��Ʈ(0)�� ����մϴ�.
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
    // �� �޽� ��(�ﰢ��)�� ���캸�� �ش� ���� �ε����� �˻�
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // face�� ��� �ε����� �˻��ϰ� �ε��� ���Ϳ� ����
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // ���̴��� ���÷� �̸��� ���� ��Ģ�� �����մϴ�. �� Ȯ�� �ؽ�ó�� �̸��� �����ؾ� �մϴ�.
    // 'texture_diffuseN'���� ���⿡�� N�� 1���� MAX_SAMPLER_NUMBER������ �Ϸ� ��ȣ�Դϴ�.
    // ���� ����� ����� ��ó�� �ٸ� �ؽ�ó���� �����ϰ� ����˴ϴ�.
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

    // ����� Mesh �����Ϳ��� ������ Mesh ��ü ��ȯ
    return Mesh(vertices, indices, textures, this);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // �ؽ�ó�� ������ �ε�Ǿ����� Ȯ���ϰ� �׷��ٸ� �� �ؽ�ó �ε带 �ǳʶݴϴ�.
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // ������ ���� ��θ� ���� �ؽ�ó�� �̹� �ε�Ǿ����� ��ŵ(����ȭ)
                break;
            }
        }
        if (!skip)
        {   // �ؽ�ó�� ���� �ε���� ���� ��� �ε�
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->m_directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); //�ߺ� �ؽ�ó�� ���ʿ��ϰ� �ε����� �ʵ��� ��ü �𵨿� ���� �ε�� �ؽ�ó�� ����
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