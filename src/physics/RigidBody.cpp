#include "RigidBody.h"

using Octo::RigidBody;

RigidBody::RigidBody(const BodyMode mode, const JPH::ObjectLayer layer, const bool activate)
    : m_Mode(mode), m_Layer(layer), m_Activate(activate)
{
    m_bodyInterface = Physics::getBodyInterface();
}

void RigidBody::setCollider(Collider* collider)
{
    const JPH::BodyCreationSettings bodySettings(
        collider->getShape().geJoltShape(),
        JPH::RVec3(0, 0, 0),
        JPH::Quat::sIdentity(), 
        convertBodyMode(m_Mode), 
        m_Layer
    );
    
    m_JoltBody = m_bodyInterface->CreateBody(bodySettings);
    m_bodyInterface->AddBody(m_JoltBody->GetID(), (m_Activate ? JPH::EActivation::Activate : JPH::EActivation::DontActivate) );
}

RigidBody::~RigidBody()
{
    m_bodyInterface->RemoveBody(m_JoltBody->GetID());
    m_bodyInterface->DestroyBody(m_JoltBody->GetID());
}

void RigidBody::setLinearVelocity(const glm::vec3 velocity) const
{
    m_bodyInterface->SetLinearVelocity(m_JoltBody->GetID(), JPH::Vec3(velocity.x, velocity.y, velocity.z));
    //m_JoltBody->SetLinearVelocity(JPH::Vec3(velocity.x, velocity.y, velocity.z));
}

void RigidBody::setAngularVelocity(const glm::vec3 velocity) const
{
    m_bodyInterface->SetAngularVelocity(m_JoltBody->GetID(), JPH::Vec3(velocity.x, velocity.y, velocity.z));
}

void RigidBody::addForce(const glm::vec3 force) const
{
    m_bodyInterface->AddForce(m_JoltBody->GetID(), JPH::Vec3(force.x, force.y, force.z));
}

void RigidBody::addForce(const glm::vec3 force, const glm::vec3 direction) const
{
    m_bodyInterface->AddForce(m_JoltBody->GetID(), JPH::Vec3(force.x, force.y, force.z), JPH::Vec3(direction.x, direction.y, direction.z));
}

void RigidBody::addImpulse(const glm::vec3 impulse) const
{
    m_bodyInterface->AddImpulse(m_JoltBody->GetID(), JPH::Vec3(impulse.x, impulse.y, impulse.z));
}

void RigidBody::setPosition(const glm::vec3 position) const
{
    m_bodyInterface->SetLinearAndAngularVelocity(m_JoltBody->GetID(), JoltUtils::convertGlmVec3({0, 0, 0}), JoltUtils::convertGlmVec3({0, 0, 0}));
    m_bodyInterface->SetPosition(m_JoltBody->GetID(), JoltUtils::convertGlmVec3(position), JPH::EActivation::Activate);
}

void RigidBody::setRotation(const glm::quat rotation) const
{
    m_bodyInterface->SetRotation(m_JoltBody->GetID(), JoltUtils::convertGlmQuat(rotation), JPH::EActivation::Activate);
}

glm::vec3 RigidBody::getPosition() const
{
    //auto pos = m_JoltBody->GetPosition();
    //auto pos = m_bodyInterface->GetPosition(m_JoltBody->GetID());

    //return {pos.GetX(), pos.GetY(), pos.GetZ()};

    return JoltUtils::convertJoltVec3(m_bodyInterface->GetPosition(m_JoltBody->GetID()));
}

glm::quat RigidBody::getRotation() const
{
    //auto rot = m_JoltBody->GetRotation();
   // auto rot = m_bodyInterface->GetRotation(m_JoltBody->GetID());

   // return { rot.GetW(), rot.GetX(), rot.GetY(), rot.GetZ() };
   return JoltUtils::convertJoltQuat(m_bodyInterface->GetRotation(m_JoltBody->GetID()));
}

JPH::EMotionType RigidBody::convertBodyMode(const BodyMode mode)
{
    switch (mode)
        {
        case BodyMode::Static:
            return JPH::EMotionType::Static;

        case BodyMode::Dynamic:
            return JPH::EMotionType::Dynamic;

        case BodyMode::Kinematic:
            return JPH::EMotionType::Kinematic;
    }

    return JPH::EMotionType::Static;
}