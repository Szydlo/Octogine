#include "LightingManager.h"

using Octo::LightingManager;

void LightingManager::updateLights(Shader* shader)
{
    if (m_DirLight)
    {
        m_DirLight->setShader(shader);
    }

    shader->setInt("currentPointLights", m_PointLights.size());
    shader->setInt("currentSpotLights", m_SpotLights.size());

    for (int i = 0; i < m_PointLights.size(); i++)
    {
        m_PointLights[i]->setShader(shader, i); 
    }

    for (int i = 0; i < m_SpotLights.size(); i++)
    {
        m_SpotLights[i]->setShader(shader, i);
    }
}

void LightingManager::updateShadow(Shader* shader)
{
    if (m_Shadow)
    {
        shader->bind();
        m_Shadow->getDepthTexture().bind();

        shader->setInt("shadowMap", 0);
        shader->setMat4("lightSpaceMatrix", m_Shadow->getLightSpaceMatrix());
        shader->unbind();
    }
}