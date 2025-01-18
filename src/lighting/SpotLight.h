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

        void setShader(Shader* shader, int id)
        {
            shader->setVec3("spotLight.position", position);
            shader->setVec3("spotLight.direction", direction);
            
            shader->setFloat("spotLight.constant", constant);
            shader->setFloat("spotLight.linear", linear);
            shader->setFloat("spotLight.quadratic", quadratic);

            shader->setVec3("spotLight.ambient", ambient);
            shader->setVec3("spotLight.specular", specular);
            shader->setVec3("spotLight.diffuse", diffuse);

            shader->setFloat("spotLight.cutOff", cutOff);
            shader->setFloat("spotLight.outerCutOff", outerCutOff);
        }

    };
};