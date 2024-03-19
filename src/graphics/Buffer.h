#pragma once

#include "glad/glad.h"

namespace Octo
{
    enum class BufferType
    {
        Array = GL_ARRAY_BUFFER,
        Element = GL_ELEMENT_ARRAY_BUFFER,
    };

    enum class DrawMode
    {
        STATIC = GL_STATIC_DRAW,
        DYNAMIC = GL_DYNAMIC_DRAW,
    };

    class Buffer
    {
        public:
            Buffer(BufferType type);
            ~Buffer();

            void bind();
            void unbind();

            void setData(size_t size, void* data, DrawMode mode);

            unsigned int getCount() { return m_Count; }
        private:
            unsigned int m_Count; // only for element buffer
            unsigned int m_Identity;
            BufferType m_Type;
    };
}
