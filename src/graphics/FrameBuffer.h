#pragma once

#include "glad/glad.h"

#include "Texture2D.h"

namespace Octo
{
    class FrameBuffer
    {
        public:
            FrameBuffer();
            ~FrameBuffer();

            void setTexture2D(Texture2D& texture);

            void drawBuffer(int buffer);
            void readBuffer(int buffer);

            void bind();
            void unbind();
        private:
            unsigned int m_Identity;
    };
}
