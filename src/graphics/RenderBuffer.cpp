#include "RenderBuffer.h"

using Octo::RenderBuffer;

RenderBuffer::RenderBuffer()
{
    glGenRenderbuffers(1, &m_Identity);
}

RenderBuffer::~RenderBuffer()
{
    glDeleteRenderbuffers(1, &m_Identity);
}

void RenderBuffer::bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Identity);
}

void RenderBuffer::unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderBuffer::renderBufferStorage(glm::vec2 size)
{
    bind();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size.x, size.y);
}


void RenderBuffer::attachFrameBuffer(const FrameBuffer &frameBuffer) const
{
    frameBuffer.bind();
    bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_Identity);
}