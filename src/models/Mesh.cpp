#include "Mesh.h"

using Octo::Mesh;

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const Material& material)
: /*material("../../../assets/textures/gold/gold_albedo.png",
    "../../../assets/textures/gold/gold_normal.png",
    "../../../assets/textures/gold/gold_mettalic.png",
    "../../../assets/textures/gold/roug.png",
    "../../../assets/textures/gold/gold_mettalic.png"
        ),*/ 
    m_VBO(BufferType::Array),
    m_EBO(BufferType::Element),
    m_Material(material)
{
    m_VAO.bind();

    m_VBO.bind();
    m_VBO.setData(vertices.size() * sizeof(Vertex), vertices.data(), DrawMode::STATIC);

    m_EBO.bind();
    m_EBO.setData(indices.size() * sizeof(unsigned int), indices.data(), DrawMode::STATIC);

    m_VAO.setAttribute(static_cast<int>(Attributes::Pos), 3, sizeof(Vertex), offsetof(Vertex, Vertex::position));
    m_VAO.setAttribute(static_cast<int>(Attributes::Tex), 2, sizeof(Vertex), offsetof(Vertex, Vertex::txtCoord));
    m_VAO.setAttribute(static_cast<int>(Attributes::Normal), 3, sizeof(Vertex), offsetof(Vertex, Vertex::normal));

    m_VAO.setAttribute(static_cast<int>(Attributes::BoneIDs), 4, sizeof(Vertex), offsetof(Vertex, Vertex::boneIDs));
    m_VAO.setAttribute(static_cast<int>(Attributes::Weights), 4, sizeof(Vertex), offsetof(Vertex, Vertex::weights));

    m_VAO.setAttribute(static_cast<int>(Attributes::Tangent), 3, sizeof(Vertex), offsetof(Vertex, Vertex::tangent));
    m_VAO.setAttribute(static_cast<int>(Attributes::Bitangent), 3, sizeof(Vertex), offsetof(Vertex, Vertex::bitangent));


    m_VAO.unbind();
    m_EBO.unbind();
    m_VBO.unbind();
}

void Mesh::draw(Shader& shader, const glm::mat4 &model)
{
    m_Material.setShader(&shader);
    Renderer::basicDraw(m_VAO, shader, &m_Material, m_EBO.getCount(), model);
}