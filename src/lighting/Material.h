#pragma once

#include "glm/glm.hpp"

#include "../graphics/Shader.h"
#include "../graphics/Texture2D.h"

namespace Octo
{
    struct Material
    {
        Material(std::string albPath, std::string normPath)
            : albedoTXT(albPath), normalTXT(normPath, TextureColor::Normal)
        {

        }

        void setShader(Shader* shader)
        {
            albedoTXT.bind(1);
            shader->setInt("material.albedoTXT", 1);
            normalTXT.bind(2);
            shader->setInt("material.normalTXT", 2);
        }

        glm::vec3 albedoColor = glm::vec3(1.0, 1.0, 1.0);
        Texture2D albedoTXT;
        Texture2D normalTXT;
    };
};