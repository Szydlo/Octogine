#pragma once

#include "glm/glm.hpp"

#include "../graphics/Shader.h"

namespace Octo 
{
    struct Light
    {
        glm::vec3 diffuse = glm::vec3(0.3);    
        glm::vec3 ambient = glm::vec3(0.3);
        glm::vec3 specular = glm::vec3(0.3);

        virtual void setShader(Shader& shader) 
        {}
    };
};