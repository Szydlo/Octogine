#pragma once

#include "glad/glad.h"

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

            void bind();
            void unbind();

            void setAttribute(unsigned int index, int size, unsigned int stride, unsigned int offset);
            void setIAttribute(unsigned int index, int size, unsigned int stride, unsigned int offset);
            void setAttribDivisior(unsigned int index, unsigned int divisior);
        private:
            unsigned int m_Identity;
    };
};