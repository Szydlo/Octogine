#include "Physics.h"

using Octo::Physics;

void Physics::init()
{
    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();

    m_Allocator = new JPH::TempAllocatorImpl(PhysicsAllocatorSize);
    m_JobSystem = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, JPH::thread::hardware_concurrency() - 1);

    m_PhysicsSystem = new JPH::PhysicsSystem();
    m_PhysicsSystem->Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, m_BPLayerInterface, m_ObjectVsBrodPhaseFilter, m_ObjectLayerPair);

    m_BodyInterface = &m_PhysicsSystem->GetBodyInterface();
    
}

void Physics::update(double deltaTime)
{
    const int cCollisionSteps = 1;
    m_PhysicsSystem->Update(cDeltaTime, cCollisionSteps, m_Allocator, m_JobSystem);
}

void Physics::destroy()
{
    JPH::UnregisterTypes();
	JPH::Factory::sInstance = nullptr;

    delete m_Allocator;
    delete m_JobSystem;
    delete m_PhysicsSystem;
}