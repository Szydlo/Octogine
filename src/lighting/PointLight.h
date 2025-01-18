#pragma once

#include "Light.h"

namespace Octo
{
    struct PointLight : public Light
    {
        glm::vec3 position = glm::vec3(0);

        float constant = 0;
        float linear = 0;
        float quadratic = 0;

        void setShader(Shader& shader) override
        {
            shader.setVec3("pointLight.position", position);
            
            shader.setFloat("pointLight.constant", constant);
            shader.setFloat("pointLight.linear", linear);
            shader.setFloat("pointLight.quadratic", quadratic);

            shader.setVec3("pointLight.ambient", ambient);
            shader.setVec3("pointLight.specular", specular);
            shader.setVec3("pointLight.diffuse", diffuse);
        }

        void setShader(Shader* shader, int id)
        {
            shader->setVec3("pointLights[" + std::to_string(id) + "].position", position);
            
            shader->setFloat("pointLights[" + std::to_string(id) + "].constant", constant);
            shader->setFloat("pointLights[" + std::to_string(id) + "].linear", linear);
            shader->setFloat("pointLights[" + std::to_string(id) + "].quadratic", quadratic);

            shader->setVec3("pointLights[" + std::to_string(id) + "].ambient", ambient);
            shader->setVec3("pointLights[" + std::to_string(id) + "].specular", specular);
            shader->setVec3("pointLights[" + std::to_string(id) + "].diffuse", diffuse);
        } 
    };
};