#include "FrameBuffer.h"

#include "glad/glad.h"

using Octo::FrameBuffer;

FrameBuffer::FrameBuffer()
{
    glGenFramebuffers(1, &m_Identity);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_Identity);
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_Identity);
}

void FrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::setTexture2D(Texture2D& texture)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<int>(texture.getAttachment()), GL_TEXTURE_2D, texture.getIdentity(), 0);
}

void FrameBuffer::setCubeMapFace(Cubemap &cubemap, unsigned int id)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + id, cubemap.getIdentity(), 0);
}


void FrameBuffer::drawBuffer(const int buffer)
{
    glDrawBuffer(buffer);
}

void FrameBuffer::readBuffer(const int buffer)
{
    glReadBuffer(buffer);
}