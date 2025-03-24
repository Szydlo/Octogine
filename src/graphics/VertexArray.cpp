#include "VertexArray.h"

using Octo::VertexArray;

VertexArray::VertexArray()
{
    glCreateVertexArrays(1, &m_Identity);
	bind();
}

VertexArray::~VertexArray()
{
	// TODO @ FIX IT SOMEDAY IT'S RAHTER MODEL.CPP ISSUE THAN THIS DECONSTRUCTOR
   //glDeleteVertexArrays(1, &m_Identity);
}

void VertexArray::bind() const
{
    glBindVertexArray(m_Identity);
}

void VertexArray::unbind()
{
    glBindVertexArray(0);
}

void VertexArray::setAttribute(const unsigned int index, const int size, const unsigned int stride, const unsigned int offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
}

void VertexArray::setIAttribute(const unsigned int index, const int size, const unsigned int stride, const unsigned int offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribIPointer(index, size, GL_FLOAT, stride, (void*)(offset));
}

void VertexArray::setAttribDivisior(const unsigned int index, const unsigned int divisior)
{
	glVertexAttribDivisor(index, divisior);
}