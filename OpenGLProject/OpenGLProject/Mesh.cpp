#include "Mesh.h"
#include "shader.h"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, Transform* parent)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    m_parent = parent;

    // 이제 필요한 모든 데이터가 있으므로 정점 버퍼와 해당 속성 포인터를 설정
    setupMesh();
}

void Mesh::Draw(Shader& shader)
{
    // 텍스처 바인딩
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // 바인딩 전 텍스처 유닛 활성화
        // 텍스처 번호 검색(diffuse_textureN의 N)
        string number;
        string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++); // unsigned int를 string으로 전송
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // unsigned int를 string으로 전송
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // unsigned int를 string으로 전송
        else if (name == "texture_height")
            number = std::to_string(heightNr++); //unsigned int를 string으로 전송

        // 올바른 텍스처 유닛으로 설정
        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        // 텍스터 바인딩
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // Mesh 그리기
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // 기본 값으로 설정
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
    // buffers/arrays 생성
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // vertex buffers에 데이터 로드
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 구조체의 좋은 점은 모든 항목에 대해 메모리 레이아웃이 순차적이라는 것입니다.
    // 그 효과는 단순히 구조체에 대한 포인터를 전달할 수 있다는 것이며 이는 glm::vec3/2 배열로 완벽하게 변환됩니다.
    // 다시 3/2 부동 소수점으로 변환하여 바이트 배열로 변환합니다.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex attribute pointers 설정
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    // ids
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
    // weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
    glBindVertexArray(0);
}