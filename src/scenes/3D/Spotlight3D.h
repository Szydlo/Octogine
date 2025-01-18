#pragma once

#include "Node3D.h"

namespace Octo 
{
    class Spotlight3D : public Node3D
    {
        public:
            Spotlight3D(std::string name)
            : Node3D(name)
            {}

            void onStart() override
            {
                LightingManager::addSpotLight(&m_SpotLight);
                //Renderer::setSpotLight(m_SpotLight);
            }

            void onUpdate(double deltaTime) override
            {
                Node3D::onUpdate(deltaTime);

                m_SpotLight.position = m_Transform.position;
                m_SpotLight.direction = m_Transform.rotation;

                m_SpotLight.constant = constant;
                m_SpotLight.linear = linear;
                m_SpotLight.quadratic = quadratic;
                m_SpotLight.cutOff = cutOff;
                m_SpotLight.outerCutOff = outerCutOff;

                m_SpotLight.ambient = ambient;
                m_SpotLight.diffuse = diffuse;
                m_SpotLight.specular = specular;
            }

            glm::vec3 ambient = glm::vec3(0.0f);
            glm::vec3 diffuse = glm::vec3(0.0f);
            glm::vec3 specular = glm::vec3(0.0f);


            float constant = 0;
            float linear = 0;
            float quadratic = 0;
            float cutOff = 0;
            float outerCutOff = 0;
        private:
            SpotLight m_SpotLight;

    };  
};