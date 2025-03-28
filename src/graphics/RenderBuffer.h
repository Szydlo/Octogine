#pragma once

#include "FrameBuffer.h"

namespace Octo
{
    class RenderBuffer
    {
    public:
        RenderBuffer();
        ~RenderBuffer();

        void attachFrameBuffer(const FrameBuffer& frameBuffer) const;

        void bind() const;
        void unbind() const;
    private:
        unsigned int m_Identity = 0;
    };
}