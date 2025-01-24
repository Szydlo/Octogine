#include "Mesh.h"

using Octo::Mesh;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
    : m_VBO(BufferType::Array), m_EBO(BufferType::Element),
    material("../../../assets/textures/brick_albedo.png", "../../../assets/textures/brick_normal.png")
{
    m_VAO.bind();

    m_VBO.bind();
    m_VBO.setData(vertices.size() * sizeof(Vertex), vertices.data(), DrawMode::STATIC);

    m_EBO.bind();
    m_EBO.setData(indices.size() * sizeof(unsigned int), indices.data(), DrawMode::STATIC);

    m_VAO.setAttribute((int)Attributes::Pos, 3, sizeof(Vertex), offsetof(Vertex, Vertex::position));
    m_VAO.setAttribute((int)Attributes::Tex, 2, sizeof(Vertex), offsetof(Vertex, Vertex::txtCoord));
    m_VAO.setAttribute((int)Attributes::Normal, 3, sizeof(Vertex), offsetof(Vertex, Vertex::normal));

    m_VAO.setAttribute((int)Attributes::BoneIDs, 4, sizeof(Vertex), offsetof(Vertex, Vertex::boneIDs));
    m_VAO.setAttribute((int)Attributes::Weights, 4, sizeof(Vertex), offsetof(Vertex, Vertex::weights));

    m_VAO.setAttribute((int)Attributes::Tangent, 3, sizeof(Vertex), offsetof(Vertex, Vertex::tangent));
    m_VAO.setAttribute((int)Attributes::Bitangent, 3, sizeof(Vertex), offsetof(Vertex, Vertex::bitangent));


    m_VAO.unbind();
    m_EBO.unbind();
    m_VBO.unbind();
}

void Mesh::draw(Shader& shader, glm::mat4 model)
{
    material.setShader(&shader);
    Renderer::basicDraw(m_VAO, shader, m_EBO.getCount(), model);
}