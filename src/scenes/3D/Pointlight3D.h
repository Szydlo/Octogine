#pragma once

#include "Node3D.h"

namespace Octo 
{
    class Pointlight3D : public Node3D
    {
        public:
            Pointlight3D(std::string name)
            : Node3D(name)
            {}

            void onStart() override
            {
                LightingManager::addPointLight(&m_PointLight);
                //Renderer::setSpotLight(m_SpotLight);
            }

            void onUpdate(double deltaTime) override
            {
                Node3D::onUpdate(deltaTime);

                m_PointLight.position = m_Transform.position;

                m_PointLight.constant = constant;
                m_PointLight.linear = linear;
                m_PointLight.quadratic = quadratic;

                m_PointLight.ambient = ambient;
                m_PointLight.diffuse = diffuse;
                m_PointLight.specular = specular;
            }

            glm::vec3 ambient = glm::vec3(0.0f);
            glm::vec3 diffuse = glm::vec3(0.0f);
            glm::vec3 specular = glm::vec3(0.0f);


            float constant = 0;
            float linear = 0;
            float quadratic = 0;
        private:
            PointLight m_PointLight;
    };  
};