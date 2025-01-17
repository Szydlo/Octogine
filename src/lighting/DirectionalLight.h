#pragma once

#include "Light.h"

namespace Octo
{
    struct DirectionalLight : public Light
    {
        glm::vec3 direction = glm::vec3(-2.0f, 4.0f, -1.0f);

        void setShader(Shader& shader) override
        {
            shader.setVec3("dirLight.direction", direction);
            shader.setVec3("dirLight.ambient", ambient);
            shader.setVec3("dirLight.specular", specular);
            shader.setVec3("dirLight.diffuse", diffuse);
        }

        void setShader(Shader* shader)
        {
            shader->setVec3("dirLight.direction", direction);
            shader->setVec3("dirLight.ambient", ambient);
            shader->setVec3("dirLight.specular", specular);
            shader->setVec3("dirLight.diffuse", diffuse);
        }
    };
};