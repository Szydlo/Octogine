#pragma once

#include "glm/glm.hpp"

#include "../graphics/Shader.h"

namespace Octo 
{
    struct Light
    {
        glm::vec3 color = glm::vec4(1.0);

        virtual void setShader(Shader& shader) {}
    };
};