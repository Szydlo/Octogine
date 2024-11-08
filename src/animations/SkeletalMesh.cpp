#include "SkeletalMesh.h"

using Octo::SkeletalMesh;

SkeletalMesh::SkeletalMesh(std::vector<SVertex> vertices, std::vector<unsigned int> indices, std::string txtPath)
    : m_Texture(txtPath), m_VBO(BufferType::Array), m_EBO(BufferType::Element)
{
    m_VAO.bind();

    m_VBO.bind();
    m_VBO.setData(vertices.size() * sizeof(SVertex), vertices.data(), DrawMode::STATIC);

    m_EBO.bind();
    m_EBO.setData(indices.size() * sizeof(unsigned int), indices.data(), DrawMode::STATIC);

    m_VAO.setAttribute((int)SAttributes::Pos, 3, sizeof(SVertex), offsetof(SVertex, SVertex::position));
    m_VAO.setAttribute((int)SAttributes::Tex, 2, sizeof(SVertex), offsetof(SVertex, SVertex::txtCoord));
    m_VAO.setAttribute((int)SAttributes::Normal, 3, sizeof(SVertex), offsetof(SVertex, SVertex::normal));

    m_VAO.setAttribute((int)SAttributes::Tangent, 3, sizeof(SVertex), offsetof(SVertex, SVertex::tangent));
    m_VAO.setAttribute((int)SAttributes::Bitangent, 3, sizeof(SVertex), offsetof(SVertex, SVertex::bitangent));

    m_VAO.setAttribute((int)SAttributes::BoneIDs, 4, sizeof(SVertex), offsetof(SVertex, SVertex::boneIDs));
    m_VAO.setAttribute((int)SAttributes::Weights, 4, sizeof(SVertex), offsetof(SVertex, SVertex::weights));

    m_VBO.unbind();
    m_VAO.unbind();
    m_EBO.unbind();
}

void SkeletalMesh::draw(Shader& shader, glm::mat4 model)
{
    Renderer::basicDraw(m_VAO, shader, m_Texture, m_EBO.getCount(), model);
}