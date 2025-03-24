#pragma once

#include "../graphics/FrameBuffer.h"
#include "../graphics/Texture2D.h"
#include "../graphics/Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Octo
{
    class Shadow
    {
        public:
            explicit Shadow(glm::vec2 resolution);

            void startPass(glm::vec3 lightDirection);
            void endPass();

            Texture2D& getDepthTexture() { return m_DepthTxt; }
            [[nodiscard]] glm::mat4 getLightSpaceMatrix() const { return m_LightSpaceMatrix; }
            Shader& getDepthShader() { return m_DepthShader; }
            FrameBuffer& getDepthFrameBuffer() { return m_DepthBuffer;}
        private:
            FrameBuffer m_DepthBuffer;
            Texture2D m_DepthTxt;
            Shader m_DepthShader;
            glm::vec2 m_Resolution;

            glm::mat4 m_LightSpaceMatrix = {};
    };
};