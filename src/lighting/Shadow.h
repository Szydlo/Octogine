#pragma once

#include "../graphics/FrameBuffer.h"
#include "../graphics/Texture2D.h"

namespace Octo
{
    class Shadow
    {
        public:
            Shadow(glm::vec2 resolution);

            void startPass();
            void endPass();

            Texture2D& getDepthTexture() { return m_DepthTxt; }
        private:
            FrameBuffer m_DepthBuffer;
            Texture2D m_DepthTxt;
            glm::vec2 m_Resolution;
    };
};