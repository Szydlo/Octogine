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
            RigidBody(Collider& collider, BodyMode mode, JPH::ObjectLayer layer, bool activate);
            ~RigidBody();

            glm::vec3 getPosition();
            glm::quat getRotation();

            void setPosition(glm::vec3 position);
            void setRotation(glm::quat rotation);

            void setLinearVelocity(glm::vec3 velocity);
            void setAngularVelocity(glm::vec3 velocity);
            void addForce(glm::vec3 force);
            void addForce(glm::vec3 force, glm::vec3 direction);
            void addImpulse(glm::vec3 impulse);
        private:
            JPH::EMotionType convertBodyMode(BodyMode mode);

            JPH::Body* m_JoltBody;
            JPH::BodyInterface* m_bodyInterface;
            BodyMode m_Mode;
    };
};