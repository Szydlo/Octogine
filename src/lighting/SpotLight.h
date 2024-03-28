#pragma once

#include "Light.h"

namespace Octo
{
    struct SpotLight : public Light
    {
        glm::vec3 position;
        glm::vec3 direction;

        float constant;
        float linear;
        float quadratic;

        float cutOff;
        float outerCutOff;

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
    };
};