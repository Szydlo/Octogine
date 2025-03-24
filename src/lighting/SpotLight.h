#pragma once

#include "Light.h"

namespace Octo
{
    struct SpotLight : public Light
    {
        glm::vec3 position = glm::vec3(0);
        glm::vec3 direction = glm::vec3(0);

        float constant = 0;
        float linear = 0;
        float quadratic = 0;

        float cutOff = 0;
        float outerCutOff = 0;

        void setShader(Shader& shader) override
        {
            shader.setVec3("spotLight.position", position);
            shader.setVec3("spotLight.direction", direction);
            
            shader.setFloat("spotLight.constant", constant);
            shader.setFloat("spotLight.linear", linear);
            shader.setFloat("spotLight.quadratic", quadratic);

            shader.setVec3("spotLight.ambient", ambient);
            shader.setVec3("spotLight.specular", specular);
            shader.setVec3("spotLight.diffuse", diffuse);

            shader.setFloat("spotLight.cutOff", cutOff);
            shader.setFloat("spotLight.outerCutOff", outerCutOff);
        }

        void setShader(Shader* shader, const int id) const
        {
            shader->setVec3("spotLights[" + std::to_string(id) + "].position", position);
            shader->setVec3("spotLights[" + std::to_string(id) + "].direction", direction);
            
            shader->setFloat("spotLights[" + std::to_string(id) + "].constant", constant);
            shader->setFloat("spotLights[" + std::to_string(id) + "].linear", linear);
            shader->setFloat("spotLights[" + std::to_string(id) + "].quadratic", quadratic);

            shader->setVec3("spotLights[" + std::to_string(id) + "].ambient", ambient);
            shader->setVec3("spotLights[" + std::to_string(id) + "].specular", specular);
            shader->setVec3("spotLights[" + std::to_string(id) + "].diffuse", diffuse);

            shader->setFloat("spotLights[" + std::to_string(id) + "].cutOff", cutOff);
            shader->setFloat("spotLights[" + std::to_string(id) + "].outerCutOff", outerCutOff);
        }

    };
};