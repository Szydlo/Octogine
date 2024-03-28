#pragma once

#include "Light.h"

namespace Octo
{
    struct DirectionalLight : public Light
    {
        glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);

        void setShader(Shader& shader) override
        {
            shader.setVec3("dirLight.direction", direction);
            shader.setVec3("dirLight.ambient", ambient);
            shader.setVec3("dirLight.specular", specular);
            shader.setVec3("dirLight.diffuse", diffuse);
        }
    };
};