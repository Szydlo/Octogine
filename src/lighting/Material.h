#pragma once

#include "glm/glm.hpp"

#include "../graphics/Shader.h"

namespace Octo
{
    struct Material
    {
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        
        float shininess;

        void setShader(Shader& shader)
        {
            shader.setFloat("material.shininess", shininess);

            shader.setVec3("material.ambient", ambient);
            shader.setVec3("material.diffuse", diffuse);
            shader.setVec3("material.specular", specular);
        }
    };
};