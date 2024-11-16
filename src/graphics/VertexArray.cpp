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

void VertexArray::bind()
{
    glBindVertexArray(m_Identity);
}

void VertexArray::unbind()
{
    glBindVertexArray(0);
}

void VertexArray::setAttribute(unsigned int index, int size, unsigned int stride, unsigned int offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
}

void VertexArray::setIAttribute(unsigned int index, int size, unsigned int stride, unsigned int offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribIPointer(index, size, GL_FLOAT, stride, (void*)(offset));
}

void VertexArray::setAttribDivisior(unsigned int index, unsigned int divisior)
{
	glVertexAttribDivisor(index, divisior);
}