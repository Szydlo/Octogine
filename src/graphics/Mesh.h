#pragma once

#include <vector>
#include <string>

#include "VertexArray.h"
#include "Buffer.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Renderer.h"

#include "../animations/Skeleton.h"

#include "glm/glm.hpp"

constexpr unsigned int cMaxBoneInfluence = 4;

namespace Octo
{
    struct Vertex {
        glm::vec3 position;
        glm::vec2 txtCoord = glm::vec2(0);
        glm::vec3 normal = glm::vec3(0);

        glm::vec3 tangent = glm::vec3(0);
        glm::vec3 bitangent = glm::vec3(0);

        int boneIDs[cMaxBoneInfluence];
        float weights[cMaxBoneInfluence];
    };

    enum class Attributes : int  {
        Pos = 0,
        Tex = 1,
        Normal = 2,
        Tangent = 3,
        Bitangent = 4,
        BoneIDs = 5,
        Weights = 6
    };

    class Mesh
    {
        public:
            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string txtPath); 

            void draw(Shader& shader, glm::mat4 model);

            VertexArray& getVAO() { return m_VAO; }
            Buffer& getVBO() { return m_VBO; }
            Buffer& getEBO() { return m_EBO; }
        private:
            Texture2D m_Texture;

            Skeleton m_Skeleton;
            VertexArray m_VAO;
            Buffer m_VBO;
            Buffer m_EBO;
    };
};