#pragma once

#include "spdlog/spdlog.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.h"
#include "Shader.h"

#include <vector>

namespace Octo
{
    class Model
    {
        public:
            Model(std::string path);
            void draw();
        private:
            void initMesh(aiNode* node, const aiScene* scene);

            Shader m_Shader;

            std::vector<Mesh> m_Meshes;
    };
};