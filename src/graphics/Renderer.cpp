#include "Renderer.h"

using Octo::Renderer;

void Renderer::basicDraw(VertexArray& vao, Shader& shader, unsigned int count, glm::mat4 model)
{
    if (!m_MainCamera) return;
    
    shader.bind();
    //txt.bind();

    shader.setMat4("projection", m_MainCamera->getProjectionMatrix());
    shader.setMat4("view", m_MainCamera->getViewMatrix());
    shader.setMat4("model", model);

    shader.setVec3("viewPos", m_MainCamera->getPosition());

    if (m_DirLight)
    {
        m_DirLight->setShader(shader);
    }

    vao.bind();

    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

    vao.unbind();
    shader.unbind();
}

void Renderer::basicDraw(VertexArray& vao, Shader& shader, Texture2D& txt, unsigned int count, glm::mat4 model)
{
    if (!m_MainCamera) return;
    
    shader.bind();
    txt.bind();

    shader.setMat4("projection", m_MainCamera->getProjectionMatrix());
    shader.setMat4("view", m_MainCamera->getViewMatrix());
    shader.setMat4("model", model);

    shader.setVec3("viewPos", m_MainCamera->getPosition());

    if (m_DirLight)
    {
        m_DirLight->setShader(shader);
    }

    vao.bind();

    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

    vao.unbind();
    shader.unbind();
}

void Renderer::drawSkyBox(SkyBox* skybox)
{   
    glDepthFunc(GL_LEQUAL);

    skybox->getShader().bind();

    glm::mat4 view = glm::mat4(glm::mat3(m_MainCamera->getViewMatrix())); // remove translation from the view matrix
    skybox->getShader().setMat4("view", view);

    skybox->getShader().setMat4("projection", m_MainCamera->getProjectionMatrix());

    skybox->getVAO().bind();
    skybox->getCubemap().bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);

    skybox->getVAO().unbind();

    glDepthFunc(GL_LESS);
}

void Renderer::startPass()
{

}

void Renderer::endPass()
{
    if (!m_SkyBox) return;

    drawSkyBox(m_SkyBox);
}

void Renderer::destroy()
{
    if (m_MainCamera) delete m_MainCamera;
    if (m_DirLight) delete m_DirLight;
    if (m_SkyBox) delete m_SkyBox;
}