#pragma once

#include "../graphics/Shader.h"
#include "../graphics/VertexArray.h"
#include "../graphics/Buffer.h"
#include "../graphics/Cubemap.h"

#include <array>

namespace Octo
{
    class SkyBox
    {
        public:
            explicit SkyBox(const std::array<std::string, 6> &paths);

            Cubemap& getCubemap() { return m_Cubemap; }
            Shader& getShader() { return m_Shader; }
            VertexArray& getVAO() { return m_VAO; }
        private:
            Cubemap m_Cubemap;
            VertexArray m_VAO;
            Buffer m_VBO;
            Shader m_Shader;
    };
};