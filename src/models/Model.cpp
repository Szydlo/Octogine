#include "Model.h"

using Octo::Model;

// @ TODO make it more flexible, mainly abillity to easily add new format other than gltf

Model::Model(std::string path)
    : m_Shader("../../../assets/shaders/skeletal.vs", "../../../assets/shaders/skeletal.fs")
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


    // @ TODO ADD MATERIAL SUPPORT
    for (auto& mesh : assets.meshes)
    {
        spdlog::info("Loading mesh: {}", mesh.name);

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        auto* vert = mesh.primitives[0].findAttribute("POSITION");
        vertices.resize(assets.accessors[vert->accessorIndex].count); // @ TODO lil hack change it someday to something better
    
        for (auto& p : mesh.primitives)
        {
            auto* positionIt = p.findAttribute("POSITION");
            auto& positionAccesor = assets.accessors[positionIt->accessorIndex];

            fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(assets, positionAccesor,
                [&](fastgltf::math::fvec3 pos, std::size_t idx)
                {
                    vertices[idx].position = glm::vec3(pos.x(), pos.y(), pos.z());
                }
            );

            auto* normalIt = p.findAttribute("NORMAL");
            auto& normalAccessor = assets.accessors[normalIt->accessorIndex];

            fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(assets, normalAccessor,
                [&](fastgltf::math::fvec3 normal, std::size_t idx)
                {
                    vertices[idx].normal = glm::vec3(normal.x(), normal.y(), normal.z());
                }
            );

            auto* texCoordIt = p.findAttribute("TEXCOORD_0");
            auto& texCoordAccesor = assets.accessors[texCoordIt->accessorIndex];

            fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec2>(assets, texCoordAccesor,
                [&](fastgltf::math::fvec2 uv, std::size_t idx)
                {
                    vertices[idx].txtCoord = glm::vec2(uv.x(), 1.0 - uv.y());
                }
            );

            auto& indexAccessor = assets.accessors[p.indicesAccessor.value()];
            
            for (auto element : fastgltf::iterateAccessor<std::uint32_t>(assets, indexAccessor))
            {
                indices.push_back(element);
            }

            auto* weightIt = p.findAttribute("WEIGHTS_0");
            auto& weightAccessor = assets.accessors[weightIt->accessorIndex];

            fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec4>(assets, weightAccessor,
                [&](fastgltf::math::fvec4 weights, std::size_t idx)
                {
                    vertices[idx].weights[0] = weights.w();
                    vertices[idx].weights[1] = weights.x();
                    vertices[idx].weights[2] = weights.y();
                    vertices[idx].weights[3] = weights.z();
            }
            );

            auto* jointIt = p.findAttribute("JOINTS_0");
            auto& jointAccessor = assets.accessors[jointIt->accessorIndex];

            fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec4>(assets, jointAccessor,
                [&](fastgltf::math::fvec4 joints, std::size_t idx)
                {
                    vertices[idx].boneIDs[0] = joints.w();
                    vertices[idx].boneIDs[1] = joints.x();
                    vertices[idx].boneIDs[2] = joints.y();
                    vertices[idx].boneIDs[3] = joints.z();
                }
            );
        }

        m_Meshes.emplace_back(vertices, indices, "../../../assets/textures/character.png");
    }

    
    for (auto& skin : assets.skins)
    {
        fastgltf::Skin;
        spdlog::info("Loading skeleton: {}", skin.name);

        m_Skeleton.name = skin.name;

        for (auto& joint : skin.joints)
        {
            auto& boneData = assets.nodes[joint];

            TBone bone;
            

            bone.name = boneData.name;

            for (auto& child : boneData.children)
            {
                bone.children.push_back((std::string)(assets.nodes[child].name));
            }

            m_Skeleton.bones.push_back(bone);
        }

        auto& invAccessor = assets.accessors[skin.inverseBindMatrices.value()];

        fastgltf::iterateAccessorWithIndex<fastgltf::math::fmat4x4>(assets, invAccessor, 
            [&](fastgltf::math::fmat4x4 transform, std::size_t idx)
            {
                glm::mat4 converted;

                for (int r = 0; r < 4; r++)
                {
                    for (int c = 0; c < 4; c++)
                    {
                        converted[r][c] = transform[r][c];
                    }
                }

                m_Skeleton.bones[idx].inverseBindMatrix = converted;
            }
        );
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