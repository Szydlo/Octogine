#pragma once

#include "Layers.h"
#include "Physics.h"
#include "Collider.h"
#include "JoltUtils.h"

#include <glm/glm.hpp>

namespace Octo
{
    enum class BodyMode : unsigned int { 
        Static, Dynamic, Kinematic
    };

    class RigidBody
    {
        public:
            RigidBody(BodyMode mode, JPH::ObjectLayer layer, bool activate);
            ~RigidBody();

            glm::vec3 getPosition();
            glm::quat getRotation();

            void setCollider(Collider* collider);

            void setPosition(glm::vec3 position);
            void setRotation(glm::quat rotation);

            void setLinearVelocity(glm::vec3 velocity);
            void setAngularVelocity(glm::vec3 velocity);
            void addForce(glm::vec3 force);
            void addForce(glm::vec3 force, glm::vec3 direction);
            void addImpulse(glm::vec3 impulse);

            JPH::Body* getJoltBody() { return m_JoltBody; }
            JPH::BodyInterface* getJoltBodyInterface() { return m_bodyInterface; }
        private:
            JPH::EMotionType convertBodyMode(BodyMode mode);

            JPH::Body* m_JoltBody = nullptr;
            JPH::BodyInterface* m_bodyInterface;
            BodyMode m_Mode;
            Collider* m_Collider;
            JPH::ObjectLayer m_Layer;
            bool m_Activate = true;
    };
};