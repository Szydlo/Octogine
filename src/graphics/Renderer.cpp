#include "Renderer.h"

using Octo::Renderer;

void Renderer::basicDraw(VertexArray& vao, Shader& shader, unsigned int count, glm::mat4 model)
{
    m_DrawQueue.push_back({&vao, &shader, count, model});
   /* if (!m_MainCamera) return;
    
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
    shader.unbind();*/
}

void Renderer::basicDraw(VertexArray& vao, Shader& shader, Texture2D& txt, unsigned int count, glm::mat4 model)
{
    m_DrawQueue.push_back({&vao, &shader, count, model});
    /*if (!m_MainCamera) return;
    
    shader.bind();
    //txt.bind();

    shader.setMat4("projection", m_MainCamera->getProjectionMatrix());
    shader.setMat4("view", m_MainCamera->getViewMatrix());
    shader.setMat4("model", model);

    //shader.setInt("txt", 0);

    shader.setVec3("viewPos", m_MainCamera->getPosition());

    if (m_DirLight)
    {
        m_DirLight->setShader(shader);
    }

    vao.bind();

    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

    vao.unbind();
    shader.unbind();*/
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
    m_DrawQueue.clear();
}

void Renderer::drawElement(DrawElement& el, Shader* shader)
{
    if (!m_MainCamera) return;
    
    shader->bind();

    shader->setMat4("projection", m_MainCamera->getProjectionMatrix());
    shader->setMat4("view", m_MainCamera->getViewMatrix());
    shader->setMat4("model", el.transform);
    shader->setVec3("viewPos", m_MainCamera->getPosition());

    LightingManager::updateLights(el.shader);

    el.vao->bind();
    glDrawElements(GL_TRIANGLES, el.count, GL_UNSIGNED_INT, 0);
    el.vao->unbind();

    shader->unbind();
}
void Renderer::endPass()
{
    Shadow* shadow = LightingManager::getShadow();

    if (shadow)
    {
        shadow->startPass(LightingManager::getDirectionalLight()->direction);

        for (auto& el : m_DrawQueue)
        {
            drawElement(el, &shadow->getDepthShader());
        }  

        shadow->endPass();
    }

    for (auto& el : m_DrawQueue)
    {
        LightingManager::updateShadow(el.shader);

        drawElement(el, el.shader);
    }

    if (!m_SkyBox) return;

    drawSkyBox(m_SkyBox);
}

void Renderer::destroy()
{
    if (m_MainCamera) delete m_MainCamera;
    if (m_SkyBox) delete m_SkyBox;
}