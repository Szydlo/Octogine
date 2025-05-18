#include "Model.h"

#include "fastgltf/core.hpp"
#include "fastgltf/types.hpp"
#include "fastgltf/tools.hpp"

#include "glm/glm.hpp"
#include "glm/geometric.hpp"

using Octo::Model;

// @ TODO make it more flexible, mainly ability to easily add new format other than gltf

Model::Model(const std::string& path, bool loadSkeleton)
    : m_Shader("../../../assets/shaders/main.vs", "../../../assets/shaders/main.fs")
{
    if (loadSkeleton)
        m_Shader = Shader("../../../assets/shaders/skeletal.vs", "../../../assets/shaders/skeletal.fs");

    fastgltf::Parser parser(
        fastgltf::Extensions::KHR_materials_unlit | fastgltf::Extensions::KHR_texture_transform
       );
    // @ TODO currently loading some extensions, idk if its correct
    std::filesystem::path fpath(path);

    auto data = fastgltf::GltfDataBuffer::FromPath(fpath);

    auto error = data.error();
    if (error != fastgltf::Error::None) {
        spdlog::error("fastgltf data error: {}\n{} ", getErrorName(error), getErrorMessage(error));
        return;
    }

    auto asset = parser.loadGltf(data.get(), fpath.parent_path(), fastgltf::Options::None);

    error = asset.error();
    if (error != fastgltf::Error::None) {
        spdlog::error("fastgltf asset error: {}\n{} ", getErrorName(error), getErrorMessage(error));
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
        //vertices.resize(assets.accessors[vert->accessorIndex].count); // @ TODO lil hack change it someday to something better

        for (auto& p : mesh.primitives)
        {
            auto* positionIt = p.findAttribute("POSITION");
            auto& positionAccesor = assets.accessors[positionIt->accessorIndex];

            vertices.resize(assets.accessors[positionIt->accessorIndex].count);

            fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(assets, positionAccesor,
                [&](fastgltf::math::fvec3 pos, const std::size_t idx) {
                    if (idx >= vertices.size()) {
                        spdlog::error("Index {} out of bounds (size: {})", idx, vertices.size());
                        return;
                    }
                    vertices[idx].position = glm::vec3(pos.x(), pos.y(), pos.z());
                }
            );


            auto* normalIt = p.findAttribute("NORMAL");
            auto& normalAccessor = assets.accessors[normalIt->accessorIndex];

            fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(assets, normalAccessor,
                [&](fastgltf::math::fvec3 normal, const std::size_t idx)
                {
                    vertices[idx].normal = glm::vec3(normal.x(), normal.y(), normal.z());
                }
            );

            auto* texCoordIt = p.findAttribute("TEXCOORD_0");
            auto& texCoordAccesor = assets.accessors[texCoordIt->accessorIndex];

            fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec2>(assets, texCoordAccesor,
                [&](fastgltf::math::fvec2 uv, const std::size_t idx)
                {
                    vertices[idx].txtCoord = glm::vec2(uv.x(), 1.0 - uv.y());
                }
            );

            // @TODO THIS SHIT IS FUCKED UP

            auto* tangentIt = p.findAttribute("TANGENT");
            auto& tangentAccessor = assets.accessors[tangentIt->accessorIndex];

            if (tangentAccessor.type == fastgltf::AccessorType::Vec4)
            {
                fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec4>(assets, tangentAccessor,
                    [&](fastgltf::math::fvec4 tang, const std::size_t idx)
                    {
                        vertices[idx].tangent = glm::vec3(tang.x(), tang.y(), tang.z());
                        vertices[idx].bitangent = glm::cross(vertices[idx].normal, vertices[idx].tangent) * tang.w();
                    }
                );
            }
            else if (tangentAccessor.type == fastgltf::AccessorType::Vec3)
            {
                fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(assets, tangentAccessor,
                    [&](fastgltf::math::fvec3 tang, const std::size_t idx)
                    {
                        vertices[idx].tangent = glm::vec3(tang.x(), tang.y(), tang.z());
                    }
                );
            }

            
            auto bitangentIt = p.findAttribute("BITANGENT");
            if (bitangentIt != p.attributes.end())  
            {
                auto& bitangentAccessor = assets.accessors[bitangentIt->accessorIndex];
                
                if (bitangentAccessor.type == fastgltf::AccessorType::Vec4)
                {
                    fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec4>(assets, bitangentAccessor,
                        [&](fastgltf::math::fvec4 bitang, const std::size_t idx)
                        {
                            vertices[idx].bitangent = glm::vec3(bitang.x(), bitang.y(), bitang.z());
                        }
                    );
                }
                else if (bitangentAccessor.type == fastgltf::AccessorType::Vec3)
                {
                    fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(assets, bitangentAccessor,
                        [&](fastgltf::math::fvec3 bitang, const std::size_t idx)
                        {
                            vertices[idx].bitangent = glm::vec3(bitang.x(), bitang.y(), bitang.z());
                        }
                    );
                }
            }


            auto& indexAccessor = assets.accessors[p.indicesAccessor.value()];
            
            for (auto element : fastgltf::iterateAccessor<std::uint32_t>(assets, indexAccessor))
            {
                indices.push_back(element);
            }

            auto weightIt = p.findAttribute("WEIGHTS_0");
            
            if (weightIt != p.attributes.end())
            {
                auto& weightAccessor = assets.accessors[weightIt->accessorIndex];

                fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec4>(assets, weightAccessor,
                    [&](fastgltf::math::fvec4 weights, const std::size_t idx)
                    {
                        vertices[idx].weights[0] = weights.w();
                        vertices[idx].weights[1] = weights.x();
                        vertices[idx].weights[2] = weights.y();
                        vertices[idx].weights[3] = weights.z();
                });
            }

            auto jointIt = p.findAttribute("JOINTS_0");
            
            if (jointIt != p.attributes.end())
            {
                auto& jointAccessor = assets.accessors[jointIt->accessorIndex];

                fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec4>(assets, jointAccessor,
                    [&](fastgltf::math::fvec4 joints, const std::size_t idx)
                    {
                        vertices[idx].boneIDs[0] = joints.w();
                        vertices[idx].boneIDs[1] = joints.x();
                        vertices[idx].boneIDs[2] = joints.y();
                        vertices[idx].boneIDs[3] = joints.z();
                    });
            }
        }

        m_Meshes.emplace_back(vertices, indices);
    }

    
    if (!loadSkeleton) return;

    for (auto& skin : assets.skins)
    {
        spdlog::info("Loading skeleton: {}", skin.name);

        m_Skeleton.name = skin.name;

        for (auto& joint : skin.joints)
        {
            auto& boneData = assets.nodes[joint];

            TBone bone;
            

            bone.name = boneData.name;

            for (auto& child : boneData.children)
            {
                bone.children.push_back(static_cast<std::string>(assets.nodes[child].name));
            }

            m_Skeleton.bones.push_back(bone);
        }

        auto& invAccessor = assets.accessors[skin.inverseBindMatrices.value()];

        fastgltf::iterateAccessorWithIndex<fastgltf::math::fmat4x4>(assets, invAccessor, 
            [&](fastgltf::math::fmat4x4 transform, const std::size_t idx)
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

void Model::draw(const bool useColor)
{
    for (auto& mesh : m_Meshes)
    {
        m_Shader.bind();
        m_Shader.setBool("useColor", useColor);
        m_Shader.setVec3("color", m_Color);
		mesh.draw(m_Shader, m_Transform);
    }  
}

void Model::draw(Shader& shader)
{
    for (auto& mesh : m_Meshes)
    {
        mesh.draw(shader, m_Transform);
    }
}

Model::~Model() = default;