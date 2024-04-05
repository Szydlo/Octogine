#pragma once

#include "../graphics/FrameBuffer.h"
#include "../graphics/Texture2D.h"
#include "../graphics/Renderer.h"
#include "../graphics/Shader.h"

namespace Octo
{
    class Shadow
    {
        public:
            Shadow(glm::vec2 resolution);

            void startPass();
            void endPass();

            Texture2D& getDepthTexture() { return m_DepthTxt; }
            glm::mat4 getLightSpaceMatrix() { return m_LightSpaceMatrix; }
            Shader& getDepthShader() { return m_DepthShader; }
            FrameBuffer& getDepthFrameBuffer() { return m_DepthBuffer;}
        private:
            FrameBuffer m_DepthBuffer;
            Texture2D m_DepthTxt;
            Shader m_DepthShader;
            glm::vec2 m_Resolution;

            glm::mat4 m_LightSpaceMatrix;
    };
};