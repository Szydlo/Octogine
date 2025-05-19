#pragma once

#include <vector>

#include "../graphics/VertexArray.h"
#include "../graphics/Buffer.h"
#include "../graphics/Shader.h"
#include "../graphics/Texture2D.h"
#include "../graphics/Renderer.h"

#include "../lighting/Material.h"

#include "glm/glm.hpp"

constexpr unsigned int cMaxBoneInfluence = 4;

namespace Octo
{
    struct Vertex {
        glm::vec3 position;
        glm::vec2 txtCoord = glm::vec2(0);
        glm::vec3 normal = glm::vec3(0);

        int boneIDs[cMaxBoneInfluence] = {};
        float weights[cMaxBoneInfluence] = {};

        glm::vec3 tangent = glm::vec3(0);
        glm::vec3 bitangent = glm::vec3(0);
    };

    enum class Attributes : int  {
        Pos = 0,
        Tex = 1,
        Normal = 2,
        BoneIDs = 3,
        Weights = 4,
        Tangent = 5,
        Bitangent = 6,
    };

    class Mesh
    {
        public:
            Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const Material& material);

            void draw(Shader& shader, const glm::mat4 &model);

            VertexArray& getVAO() { return m_VAO; }
            Buffer& getVBO() { return m_VBO; }
            Buffer& getEBO() { return m_EBO; }
            private:
            Material m_Material;
            VertexArray m_VAO;
            Buffer m_VBO;
            Buffer m_EBO;

    };
};