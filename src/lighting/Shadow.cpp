#include "Shadow.h"

using Octo::Shadow;

Shadow::Shadow(glm::vec2 resolution)
    : m_DepthTxt(NULL, resolution, TextureColor::Depth), 
    m_Resolution(resolution),
    m_DepthShader("../../../assets/shaders/depth.vs", "../../../assets/shaders/depth.fs")
{
    m_DepthBuffer.bind();
    m_DepthBuffer.readBuffer(NULL);
    m_DepthBuffer.drawBuffer(NULL);
    m_DepthBuffer.setTexture2D(m_DepthTxt);
    m_DepthBuffer.unbind();
}

void Shadow::startPass(glm::vec3 lightDirection)
{
    glm::mat4 lightProjection, lightView;

    float near_plane = 1.0f, far_plane = 7.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightDirection, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    m_LightSpaceMatrix = lightProjection * lightView;

    m_DepthShader.bind();
    m_DepthShader.setMat4("lightSpaceMatrix", m_LightSpaceMatrix);

    m_DepthBuffer.bind();
    glViewport(0, 0, m_Resolution.x, m_Resolution.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Shadow::endPass()
{
    m_DepthBuffer.unbind();
    glViewport(0, 0, 1920, 1080); // PLACEHOLDER VALUE @TODO CHANGE IT
}