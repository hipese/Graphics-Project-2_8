#include "Mesh.h"
#include "shader.h"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, Transform* parent)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    m_parent = parent;

    // ���� �ʿ��� ��� �����Ͱ� �����Ƿ� ���� ���ۿ� �ش� �Ӽ� �����͸� ����
    setupMesh();
}

void Mesh::Draw(Shader& shader)
{
    // �ؽ�ó ���ε�
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // ���ε� �� �ؽ�ó ���� Ȱ��ȭ
        // �ؽ�ó ��ȣ �˻�(diffuse_textureN�� N)
        string number;
        string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++); // unsigned int�� string���� ����
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // unsigned int�� string���� ����
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // unsigned int�� string���� ����
        else if (name == "texture_height")
            number = std::to_string(heightNr++); //unsigned int�� string���� ����

        // �ùٸ� �ؽ�ó �������� ����
        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        // �ؽ��� ���ε�
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // Mesh �׸���
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // �⺻ ������ ����
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
    // buffers/arrays ����
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // vertex buffers�� ������ �ε�
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // ����ü�� ���� ���� ��� �׸� ���� �޸� ���̾ƿ��� �������̶�� ���Դϴ�.
    // �� ȿ���� �ܼ��� ����ü�� ���� �����͸� ������ �� �ִٴ� ���̸� �̴� glm::vec3/2 �迭�� �Ϻ��ϰ� ��ȯ�˴ϴ�.
    // �ٽ� 3/2 �ε� �Ҽ������� ��ȯ�Ͽ� ����Ʈ �迭�� ��ȯ�մϴ�.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex attribute pointers ����
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