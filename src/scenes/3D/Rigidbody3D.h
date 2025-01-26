#pragma once

#include "Node3D.h"
#include "Collider3D.h"

namespace Octo 
{
    class Rigidbody3D : public Node3D
    {
        public:
            Rigidbody3D(std::string name)
            : Node3D(name), m_rBody(BodyMode::Dynamic, PhysicsLayers::MOVING, true)
            {}

            void onStart() override
            {
                for (auto& child : m_Children)
                {
                    if (child->isA<Collider3D>())
                    {
                        Collider& collider = child->as<Collider3D*>()->getCollider();
                        m_rBody.setCollider(&collider);
                    }
                }
            }

            void onUpdate(double deltaTime) override
            {
                if (m_rBody.getJoltBody() == nullptr) return;
                    
                getTransform().position = m_rBody.getPosition();
            }

            RigidBody& getBody() { return m_rBody; }
        private:
            RigidBody m_rBody;
    };  
};