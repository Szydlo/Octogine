#include "LightingManager.h"

using Octo::LightingManager;

void LightingManager::updateLights(Shader* shader)
{
    if (m_DirLight)
    {
        m_DirLight->setShader(shader);
    }

    for (int i = 0; i < NR_MAX_LIGHTS; i++)
    {
        // TODO @ TEMPORARY SOLUTION FOR NOW
        PointLight pl;
        SpotLight sl;
        pl.setShader(shader, i);
        sl.setShader(shader, i);
    }

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