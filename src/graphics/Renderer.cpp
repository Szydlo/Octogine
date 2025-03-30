#include "Renderer.h"

#include "../core/Events.h"

using Octo::Renderer;

void Renderer::basicDraw(VertexArray& vao, Shader& shader, unsigned int count, const glm::mat4 &model)
{
    m_DrawQueue.push_back({&vao, &shader, count, model});
}

void Renderer::basicDraw(VertexArray& vao, Shader& shader, Texture2D& txt, unsigned int count, const glm::mat4 &model)
{
    m_DrawQueue.push_back({&vao, &shader, count, model});
}

void Renderer::drawSkyBox(SkyBox* skybox)
{   
    glDepthFunc(GL_LEQUAL);

    skybox->getShader().bind();

    const glm::mat4 view = glm::mat4(glm::mat3(m_MainCamera->getViewMatrix())); // remove translation from the view matrix
    skybox->getShader().setMat4("view", view);

    skybox->getShader().setMat4("projection", m_MainCamera->getProjectionMatrix());

    skybox->getVAO().bind();
    skybox->getCubemap().bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);

    skybox->getVAO().unbind();

    glDepthFunc(GL_LESS);
}

void Renderer::init()
{

}


void Renderer::startPass()
{
    m_DrawQueue.clear();
    Events::onRenderStartPass();
}

void Renderer::drawElement(const DrawElement& el, const Shader* shader)
{
    if (!m_MainCamera) return;
    
    shader->bind();

    shader->setMat4("projection", m_MainCamera->getProjectionMatrix());
    shader->setMat4("view", m_MainCamera->getViewMatrix());
    shader->setMat4("model", el.transform);
    shader->setVec3("viewPos", m_MainCamera->getPosition());

    LightingManager::updateLights(el.shader);

    if (m_Enivroment)
        m_Enivroment->setShader(shader);

    el.vao->bind();
    glDrawElements(GL_TRIANGLES, el.count, GL_UNSIGNED_INT, 0);
    el.vao->unbind();

    shader->unbind();
}
void Renderer::endPass()
{
    if (Shadow* shadow = LightingManager::getShadow())
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

    if (m_SkyBox)
        drawSkyBox(m_SkyBox);

    if (m_Enivroment)
    {
        m_Enivroment->drawBackground(m_MainCamera);
    }

    Events::onRenderEndPass();
}

void Renderer::destroy()
{
    delete m_MainCamera;
    delete m_SkyBox;
    delete m_Enivroment;
}