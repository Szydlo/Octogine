#pragma once

#include "Light.h"

namespace Octo
{
    struct PointLight : public Light
    {
        glm::vec3 position;

        float constant;
        float linear;
        float quadratic;

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
    };
};