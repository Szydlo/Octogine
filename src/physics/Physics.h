#pragma once

#include <Jolt/Jolt.h>

#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include "Layers.h"

namespace Octo
{
    constexpr uint32_t PhysicsAllocatorSize = 10 * 1024 * 1024; // 10 MB

    constexpr JPH::uint cMaxBodies = 1024;
    constexpr JPH::uint cNumBodyMutexes = 0;
    constexpr JPH::uint cMaxBodyPairs = 1024;
    constexpr JPH::uint cMaxContactConstraints = 1024;
    constexpr int cCollisionSteps = 1;
    constexpr double cDeltaTime = 1.0f / 60.0f;

    class Physics 
    {
        public:
            static void init();
            static void destroy();
            static void update(double deltaTime);

            static JPH::PhysicsSystem* getPhysicsSystem() { return m_PhysicsSystem; }
            static JPH::BodyInterface* getBodyInterface() { return m_BodyInterface; }
            static void optimizeBroadPhase() { m_PhysicsSystem->OptimizeBroadPhase(); }
        private:
            inline static JPH::TempAllocatorImpl* m_Allocator;
            inline static JPH::JobSystemThreadPool* m_JobSystem;
            inline static JPH::PhysicsSystem* m_PhysicsSystem;
            inline static JPH::BodyInterface* m_BodyInterface;

            inline static BPLayerInterfaceImpl m_BPLayerInterface;
            inline static ObjectVsBroadPhaseLayerFilterImpl m_ObjectVsBrodPhaseFilter;
            inline static ObjectLayerPairFilterImpl m_ObjectLayerPair;

            inline static JPH::uint m_CurrentStep;
    };
};