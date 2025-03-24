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

            static void setTexture2D(Texture2D& texture);

            static void drawBuffer(int buffer);
            static void readBuffer(int buffer);

            void bind() const;
            static void unbind();
        private:
            unsigned int m_Identity;
    };
}
