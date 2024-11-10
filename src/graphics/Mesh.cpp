#include "Mesh.h"

using Octo::Mesh;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string txtPath)
    : m_Texture(txtPath), m_VBO(BufferType::Array), m_EBO(BufferType::Element)
{
    m_VAO.bind();

    m_VBO.bind();
    m_VBO.setData(vertices.size() * sizeof(Vertex), vertices.data(), DrawMode::STATIC);

    m_EBO.bind();
    m_EBO.setData(indices.size() * sizeof(unsigned int), indices.data(), DrawMode::STATIC);

    m_VAO.setAttribute((int)Attributes::Pos, 3, sizeof(Vertex), offsetof(Vertex, Vertex::position));
    m_VAO.setAttribute((int)Attributes::Tex, 2, sizeof(Vertex), offsetof(Vertex, Vertex::txtCoord));
    m_VAO.setAttribute((int)Attributes::Normal, 3, sizeof(Vertex), offsetof(Vertex, Vertex::normal));

   /* m_VAO.setAttribute((int)Attributes::Tangent, 3, sizeof(Vertex), offsetof(Vertex, Vertex::tangent));
    m_VAO.setAttribute((int)Attributes::Bitangent, 3, sizeof(Vertex), offsetof(Vertex, Vertex::bitangent));

    m_VAO.setAttribute((int)Attributes::BoneIDs, 4, sizeof(Vertex), offsetof(Vertex, Vertex::boneIDs));
    m_VAO.setAttribute((int)Attributes::Weights, 4, sizeof(Vertex), offsetof(Vertex, Vertex::weights));*/

    m_VBO.unbind();
    m_VAO.unbind();
    m_EBO.unbind();
}

void Mesh::draw(Shader& shader, glm::mat4 model)
{
    Renderer::basicDraw(m_VAO, shader, m_Texture, m_EBO.getCount(), model);
}