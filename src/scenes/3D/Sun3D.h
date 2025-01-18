#pragma once

#include "Node3D.h"
//#include "../lighting/DirectionalLight.h"

namespace Octo 
{
    class Sun3D : public Node3D
    {
        public:
            Sun3D(std::string name)
            : Node3D(name), m_Shadow(glm::vec2(1024))
            {}

            void onStart() override
            {
                //Renderer::setShadow(m_Shadow);
                LightingManager::setShadow(m_Shadow);
                LightingManager::setDirectionalLight(m_DirLight);
            }

            void onUpdate(double deltaTime) override
            {
                Node3D::onUpdate(deltaTime);
                m_DirLight.direction = m_Transform.rotation;
                //Renderer::setDirectionalLight(m_DirLight);
            }

            Shadow& getShadow() { return m_Shadow; }
        private:
            DirectionalLight m_DirLight;
            Shadow m_Shadow;
    };  
};