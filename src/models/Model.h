#pragma once

#include "./Mesh.h"
#include "../graphics/Shader.h"
#include "../animations/Skeleton.h"

#include "spdlog/spdlog.h"

#include <string>
#include <vector>

namespace Octo 
{
    class Model
    {
        public:
            explicit Model(const std::string& path, bool loadSkeleton = false);
            ~Model();

            void draw(bool useColor = false);
            void draw(Shader& shader);

            Shader& getShader() { return m_Shader; }
            glm::mat4& getTransform() { return m_Transform; }
            std::vector<Mesh>& getMeshes() { return m_Meshes; }
            Skeleton& getSkeleton() { return m_Skeleton; }
            [[nodiscard]] glm::vec3 getColor() const { return m_Color; }

            void setColor(const glm::vec3 color) { m_Color = color; }
            void setTransform(const glm::mat4 &transform) { m_Transform = transform; }

            std::vector<Texture2D> textures;
            std::vector<Material> materials;
        private:
            Shader m_Shader;
            Skeleton m_Skeleton;

            glm::mat4 m_Transform = glm::mat4(1.0);
            glm::vec3 m_Color = glm::vec3(1.0);
            std::vector<Mesh> m_Meshes;
    };
};