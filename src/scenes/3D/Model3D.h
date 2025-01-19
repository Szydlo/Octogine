#pragma once

#include "Node3D.h"

namespace Octo 
{
    class Model3D : public Node3D
    {
        public:
            Model3D(std::string name, std::string path)
            : Node3D(name), m_Model(path), m_Path(path) {}

            void onUpdate(double deltaTime) override
            {
                Node3D::onUpdate(deltaTime);

                m_Model.setTransform(m_Transform.getMat4());

                if (m_Parent && m_Parent->isA<Octo::Node3D>())
                {
                    Octo::Node3D* parent = m_Parent->as<Octo::Node3D*>();
                    m_Model.setTransform(m_Transform.getGlobalMat4(parent->getTransform()));
                }

                m_Model.setColor(m_Color);
                m_Model.draw(m_UseColor);
            }

            std::string& getPath() { return m_Path; }
            glm::vec3& getColor() { return m_Color; }
            void setColor(glm::vec3 color) { m_Color = color; }
        private:
            Model m_Model;
            glm::vec3 m_Color = glm::vec3(1.0);
            std::string m_Path;

            bool m_UseColor = true;
    };  
};