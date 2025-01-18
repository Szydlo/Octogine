#pragma once

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Shadow.h"

#include <array>

namespace Octo
{
    constexpr uint16_t NR_MAX_LIGHTS = 4;

    class LightingManager
    {
        public:
            static void updateLights(Shader* shader);
            static void updateShadow(Shader* shader);

            static void setDirectionalLight(DirectionalLight& light) { m_DirLight = &light; }
            static void setShadow(Shadow& shadow) { m_Shadow = &shadow; }
        
            static void addSpotLight(SpotLight* sLight) { m_SpotLights.push_back(sLight); }
            static void addPointLight(PointLight* pLight) { m_PointLights.push_back(pLight); }
        
            static DirectionalLight* getDirectionalLight() { return m_DirLight; }
            static Shadow* getShadow() { return m_Shadow; }
        private:
            inline static Shadow* m_Shadow = nullptr;
            inline static DirectionalLight* m_DirLight = nullptr;
            
            inline static std::vector<PointLight*> m_PointLights;
            inline static std::vector<SpotLight*> m_SpotLights;
    };
};