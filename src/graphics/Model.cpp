#include "Model.h"

using Octo::Model;

Model::Model(std::string path)
    : m_Shader("../../../assets/shaders/main.vs", "../../../assets/shaders/main.fs")
{
    fastgltf::Parser parser;
    std::filesystem::path fpath(path);

    auto data = fastgltf::GltfDataBuffer::FromPath(fpath);
    if (data.error() != fastgltf::Error::None) {
        spdlog::error("error");
        return;
    }

    auto asset = parser.loadGltf(data.get(), fpath.parent_path(), fastgltf::Options::None);
    if (auto error = asset.error(); error != fastgltf::Error::None) {
        spdlog::error("error");
        return;
    }

    spdlog::info("loaded model");

    auto& assets = asset.get();

    for (auto& mesh : assets.meshes)
    {
        spdlog::info("Loading mesh: {}", mesh.name);

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;


        auto* vert = mesh.primitives[0].findAttribute("POSITION");
        vertices.resize(assets.accessors[vert->accessorIndex].count);

        for (auto it = mesh.primitives.begin(); it != mesh.primitives.end(); it++)
        {
            auto* positionIt = it->findAttribute("POSITION");
            auto& positionAccesor = assets.accessors[positionIt->accessorIndex];

            fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(assets, positionAccesor,
                [&](fastgltf::math::fvec3 pos, std::size_t idx)
                {
                    vertices[idx].position = glm::vec3(pos.x(), pos.y(), pos.z());
                }
            );

            auto* normalIt = it->findAttribute("NORMAL");
            auto& normalAccessor = assets.accessors[normalIt->accessorIndex];

            fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(assets, normalAccessor,
                [&](fastgltf::math::fvec3 normal, std::size_t idx)
                {
                    vertices[idx].normal = glm::vec3(normal.x(), normal.y(), normal.z());
                }
            );

            auto* texCoordIt = it->findAttribute("TEXCOORD_0");
            auto& texCoordAccesor = assets.accessors[texCoordIt->accessorIndex];

            fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec2>(assets, texCoordAccesor,
                [&](fastgltf::math::fvec2 uv, std::size_t idx)
                {
                    vertices[idx].txtCoord = glm::vec2(uv.x(), 1.0 - uv.y());
                }
            );

            auto& indexAccessor = assets.accessors[it->indicesAccessor.value()];
            
            for (auto element : fastgltf::iterateAccessor<std::uint32_t>(assets, indexAccessor))
            {
                indices.push_back(element);
            }
        }

        m_Meshes.emplace_back(vertices, indices, "../../../assets/models/guytex.png");
    }
}

void Model::draw()
{
    for (auto& mesh : m_Meshes)
    {
		mesh.draw(m_Shader, m_Transform);
    }  
}

Model::~Model()
{

}