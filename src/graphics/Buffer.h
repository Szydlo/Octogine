#pragma once

#include "glad/glad.h"

#include "spdlog/spdlog.h"

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
            explicit Buffer(BufferType type);
            ~Buffer();

            void bind();
            void unbind();

            void setData(size_t size, const void* data, DrawMode mode);

            [[nodiscard]] unsigned int getCount() const { return m_Count; }
        private:
            unsigned int m_Count; // only for element buffer
            unsigned int m_Identity;
            BufferType m_Type;
    };
}
