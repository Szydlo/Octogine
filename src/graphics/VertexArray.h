#pragma once

#include "glad/glad.h"

#include "spdlog/spdlog.h"

/*
    @ IMPORTANT

	First bind Vertex Buffer
    then setData
    and then setAtrtributes!
*/

namespace Octo
{
    class VertexArray
    {
        public:
            VertexArray();
            ~VertexArray();

            void bind() const;
            static void unbind();

            static void setAttribute(unsigned int index, int size, unsigned int stride, unsigned int offset);

            static void setIAttribute(unsigned int index, int size, unsigned int stride, unsigned int offset);
            static void setAttribDivisior(unsigned int index, unsigned int divisior);
        private:
            unsigned int m_Identity = 0;
    };
};