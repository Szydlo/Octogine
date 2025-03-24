#include "Buffer.h"

using Octo::Buffer;

/*
    @IMPORTANT
    First unbind EBO and VBO then unbind VAO!
    (maybe not xd)
*/

Buffer::Buffer(const BufferType type)
    : m_Type(type)
{
    glGenBuffers(1, &m_Identity);
}

Buffer::~Buffer()
{
    // TODO @ FIX IT SOMEDAY IT'S RAHTER MODEL.CPP ISSUE THAN THIS DECONSTRUCTOR
    //glDeleteBuffers(1, &m_Identity);
}

void Buffer::setData(const size_t size, const void* data, DrawMode mode)
{
    glBufferData(static_cast<int>(m_Type), size, data, static_cast<int>(mode));

    if (m_Type == BufferType::Element)
    {
        m_Count = size / sizeof(unsigned int); // TODO @ A LIL HACKERY FIX IT SOMEDAY
    }
}

void Buffer::bind()
{
    glBindBuffer(static_cast<int>(m_Type), m_Identity);
}

void Buffer::unbind()
{
    glBindBuffer(static_cast<int>(m_Type), 0);
}