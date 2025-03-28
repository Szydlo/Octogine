#pragma once

#include "glm/glm.hpp"

#include "../graphics/Shader.h"
#include "../graphics/Texture2D.h"

namespace Octo
{
    struct Material
    {
        Material(const std::string &albPath, const std::string &normPath, const std::string &metPath, const std::string &roPath, const std::string &aoPath)
            : albedoTXT(albPath), normalTXT(normPath, TextureColor::Normal), metallicTXT(metPath), roughnessTXT(roPath), aoTXT(aoPath)
        {

        }

        void setShader(Shader* shader)
        {
            albedoTXT.bind(1);
            shader->setInt("material.albedoTXT", 1);
            normalTXT.bind(2);
            shader->setInt("material.normalTXT", 2);
            metallicTXT.bind(3);
            shader->setInt("material.metallicTXT", 3);
            roughnessTXT.bind(4);
            shader->setInt("material.roughnessTXT", 4);
            aoTXT.bind(5);
            shader->setInt("material.aoTXT", 5);

            shader->setVec3("material.albedo", albedo);
            
            shader->setFloat("material.metallic", metallic);
            shader->setFloat("material.roughness", roughness);
            shader->setFloat("material.ao", ao);
        }

        glm::vec3 albedo = glm::vec3(1.0, 1.0, 1.0);

        float metallic = 0;
        float roughness = 0;
        float ao = 0;

        Texture2D albedoTXT;
        Texture2D normalTXT;
        Texture2D metallicTXT;
        Texture2D roughnessTXT;
        Texture2D aoTXT;
    };
};