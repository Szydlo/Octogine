#include "Model.h"

#include "fastgltf/core.hpp"
#include "fastgltf/types.hpp"
#include "fastgltf/tools.hpp"

#include "glm/glm.hpp"
#include "glm/geometric.hpp"
#include <variant>


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
    std::filesystem::path shortPath(path);

    textures.reserve(assets.textures.size());

    for (auto& texture : assets.textures)
    {
        if (texture.imageIndex.has_value())
        {
            size_t txtIndex = texture.imageIndex.value();
            auto image = assets.images[txtIndex].data;

            
            if (std::holds_alternative<fastgltf::sources::URI>(image))
            {
                auto url = std::get<fastgltf::sources::URI>(image);
                
                auto fullPath = shortPath.remove_filename().string() + std::string(url.uri.string());
                
                textures.emplace_back(fullPath);
                //spdlog::info(fullPath);
            }
            else if(std::holds_alternative<fastgltf::sources::BufferView>(image))
            {
                //spdlog::info("fast gltf image type not supported: bufferview");
                
                auto bufferView = std::get<fastgltf::sources::BufferView>(image);
                auto viewIndex = bufferView.bufferViewIndex;

                const auto& view = assets.bufferViews[viewIndex];
                const auto& buffer = assets.buffers[view.bufferIndex];
             
                std::vector<uint8_t> imageData(view.byteLength);

                std::visit(fastgltf::visitor{
                    [&](const fastgltf::sources::Vector& vec) 
                    {
          
                    },
                    [&](const fastgltf::sources::Array& array) 
                    {
                        std::memcpy(imageData.data(), array.bytes.data() + view.byteOffset, view.byteLength);
                    },
                    [&](const auto&) 
                    {
                        spdlog::error("Unsupported buffer data source type!");
                    },
                }, buffer.data);

                textures.emplace_back(imageData.data(), imageData.size());
                //textures.emplace_back("../../../assets/models/heavy/asd/Textures/barrel.png");
            }
            else if(std::holds_alternative<fastgltf::sources::Array>(image))
            {
                textures.emplace_back("../../../assets/models/heavy/asd/Textures/barrel.png");
                spdlog::info("fast gltf image type not supported: array");
            }
            else if(std::holds_alternative<fastgltf::sources::Vector>(image))
            {
                textures.emplace_back("../../../assets/models/heavy/asd/Textures/barrel.png");
                spdlog::info("fast gltf image type not supported: vector");
            }
        } 
        else {
            spdlog::info("fastgltf no value txt");
        }
    }


    materials.reserve(assets.materials.size());
    
    for (auto& gltfMaterial : assets.materials)
    {
        auto& pbrData = gltfMaterial.pbrData;
        if (gltfMaterial.pbrData.baseColorTexture.has_value())
        {
            auto txtIndex = gltfMaterial.pbrData.baseColorTexture.value().textureIndex;
            auto& albedoTxt = textures[txtIndex];
            
            Material material(
                albedoTxt,
                albedoTxt,
                albedoTxt,
                albedoTxt,
                albedoTxt
            );
    
            /*if (gltfMaterial.pbrData.metallicRoughnessTexture.has_value())
            {
                material.metallicTXT = textures[gltfMaterial.pbrData.metallicRoughnessTexture.value().textureIndex];
                material.roughnessTXT = textures[gltfMaterial.pbrData.metallicRoughnessTexture.value().textureIndex];
            }

            if (gltfMaterial.normalTexture.has_value())
            {
                material.normalTXT = textures[gltfMaterial.normalTexture.value().textureIndex];
            }

            if (gltfMaterial.occlusionTexture.has_value())
            {
                material.aoTXT = textures[gltfMaterial.occlusionTexture.value().textureIndex];
            }*/

            material.metallic = gltfMaterial.pbrData.metallicFactor;
            material.roughness = gltfMaterial.pbrData.roughnessFactor;
    
            materials.emplace_back(material);
        }
        else 
        {
            auto& albedoTxt = textures[0];
            
            Material material(
                albedoTxt,
                albedoTxt,
                albedoTxt,
                albedoTxt,
                albedoTxt
            );
    
            material.metallic = 1.0;
            material.roughness = 1.0;
    
            materials.emplace_back(material);
        }
    }

    for (auto& mesh : assets.meshes)
    {
        spdlog::info("Loading mesh: {}", mesh.name);

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        size_t materialID = 0;

        //auto* vert = mesh.primitives[0].findAttribute("POSITION");
        //vertices.resize(assets.accessors[vert->accessorIndex].count); // @ TODO lil hack change it someday to something better

        for (auto& p : mesh.primitives)
        {
            auto* positionIt = p.findAttribute("POSITION");
            auto& positionAccesor = assets.accessors[positionIt->accessorIndex];

            vertices.clear();
            indices.clear();

            vertices.resize(assets.accessors[positionIt->accessorIndex].count);

            materialID = p.materialIndex.value();

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
                    vertices[idx].txtCoord = glm::vec2(uv.x(), uv.y());
                }
            );
 
  
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
                        if (idx < vertices.size()) // @TODO Tempopary soltuion
                        {

                            vertices[idx].tangent = glm::vec3(tang.x(), tang.y(), tang.z());
                        }
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
            
            m_Meshes.emplace_back(vertices, indices, materials[materialID]);
        }

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