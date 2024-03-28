#pragma once

#include "glm/glm.hpp"

#include "../graphics/Shader.h"

namespace Octo 
{
    struct Light
    {
        glm::vec3 diffuse = glm::vec4(1.0);    
        glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
        glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);

        virtual void setShader(Shader& shader) 
        {}
    };
};