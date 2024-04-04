#pragma once

#include "spdlog/spdlog.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.h"
#include "Shader.h"

#include "../lighting/Material.h"

#include <vector>

namespace Octo
{
    class Model
    {
        public:
            Model(std::string path, bool useColor = false, glm::vec3 color = glm::vec3(1.0));

            void draw();
            void draw(Shader& shader);

            glm::mat4 getTransform() { return m_Transform; }
            void setTransform(glm::mat4 transform) { m_Transform = transform; }

            glm::vec3 getColor() { return m_Color; }
            void setColor(glm::vec3 color) { m_Color = color; }

            Material getMaterial() { return m_Material; }
            void setMaterial(Material material) { m_Material = material; }

            Shader& getShader() { return m_Shader; }
            std::vector<Mesh>& getMeshes() { return m_Meshes; }
        private:
            void initMesh(aiNode* node, const aiScene* scene);

            Shader m_Shader;
            std::vector<Mesh> m_Meshes;

            glm::mat4 m_Transform = glm::mat4(1);

            bool m_UseColor = false;
            glm::vec3 m_Color = glm::vec3(1.0);

            Material m_Material;
    };
};