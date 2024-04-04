#include "FrameBuffer.h"

using Octo::FrameBuffer;


FrameBuffer::FrameBuffer()
{
    glGenFramebuffers(1, &m_Identity);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_Identity);
}

void FrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_Identity);
}

void FrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::setTexture2D(Texture2D& texture)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, (int)texture.getAttachment(), GL_TEXTURE_2D, texture.getIdentity(), 0);
}

void FrameBuffer::drawBuffer(int buffer)
{
    glDrawBuffer(buffer);
}

void FrameBuffer::readBuffer(int buffer)
{
    glReadBuffer(buffer);
}