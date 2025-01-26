#pragma once

#include "Node3D.h"

namespace Octo 
{
    class Collider3D : public Node3D
    {
        public:
            Collider3D(std::string name)
            : Node3D(name), m_Shape(glm::vec3(1)), 
                m_collider(m_Shape)
            {}

            void onStart() override
            {
                
            }

            void onUpdate(double deltaTime) override
            {
                
            }

            Collider& getCollider() { return m_collider; }
        private:
            BoxShape m_Shape;
            Collider m_collider;
    };  
};