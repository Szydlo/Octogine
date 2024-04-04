#include "Shadow.h"

using Octo::Shadow;

Shadow::Shadow(glm::vec2 resolution)
    : m_DepthTxt(NULL, glm::vec2(1024, 1024), TextureColor::Depth), m_Resolution(resolution)
{
    m_DepthBuffer.bind();
    m_DepthTxt.bind();

    m_DepthBuffer.setTexture2D(m_DepthTxt);
    m_DepthBuffer.drawBuffer(GL_NONE);
    m_DepthBuffer.readBuffer(GL_NONE);

    m_DepthBuffer.unbind();
    m_DepthTxt.unbind();
}

void Shadow::startPass()
{
    m_DepthBuffer.bind();
    glViewport(0, 0, 1024, 1024);
    glClear(GL_DEPTH_BUFFER_BIT);
    //m_DepthTxt.bind();
}

void Shadow::endPass()
{
    m_DepthBuffer.unbind();
    glViewport(0, 0, 1920, 1080);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}