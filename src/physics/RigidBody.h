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

            [[nodiscard]] glm::vec3 getPosition() const;
            [[nodiscard]] glm::quat getRotation() const;

            void setCollider(const Collider* collider);

            void setPosition(glm::vec3 position) const;
            void setRotation(glm::quat rotation) const;

            void setLinearVelocity(glm::vec3 velocity) const;
            void setAngularVelocity(glm::vec3 velocity) const;
            void addForce(glm::vec3 force) const;
            void addForce(glm::vec3 force, glm::vec3 direction) const;
            void addImpulse(glm::vec3 impulse) const;

            [[nodiscard]] const JPH::Body* getJoltBody() const { return m_JoltBody; }
            [[nodiscard]] const JPH::BodyInterface* getJoltBodyInterface() const { return m_bodyInterface; }
        private:
            static JPH::EMotionType convertBodyMode(BodyMode mode);

            JPH::Body* m_JoltBody = nullptr;
            JPH::BodyInterface* m_bodyInterface;
            BodyMode m_Mode;
            Collider* m_Collider = nullptr;
            JPH::ObjectLayer m_Layer;
            bool m_Activate = true;
    };
};