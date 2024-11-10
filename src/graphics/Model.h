#pragma once

#include "Mesh.h"
#include "Shader.h"

#include "spdlog/spdlog.h"
#include "fastgltf/core.hpp"
#include "fastgltf/types.hpp"
#include "fastgltf/tools.hpp"

#include <string>
#include <vector>

namespace Octo 
{
    class Model
    {
        public:
            Model(std::string path);
            ~Model();

            void draw();

            Shader& getShader() { return m_Shader; }
            glm::mat4& getTransform() { return m_Transform; }
            std::vector<Mesh>& getMeshes() { return m_Meshes; }
        private:
            Shader m_Shader;
            glm::mat4 m_Transform = glm::mat4(1.0);
            std::vector<Mesh> m_Meshes;
    };
};