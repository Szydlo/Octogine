#pragma once

#include "Node3D.h"
//#include "../lighting/DirectionalLight.h"

namespace Octo 
{
    class Sun3D : public Node3D
    {
        public:
            Sun3D(std::string name)
            : Node3D(name) {}

            void update(double deltaTime) override
            {
                Node3D::update(deltaTime);
                m_DirLight.direction = m_Transform.rotation;
                Renderer::setDirectionalLight(m_DirLight);
            }
        private:
            DirectionalLight m_DirLight;
    };  
};