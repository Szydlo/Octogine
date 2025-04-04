#pragma once

#include <Jolt/Jolt.h>

// Jolt includes
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

namespace PhysicsLayers
{
   	static constexpr JPH::ObjectLayer NON_MOVING = 0;
	static constexpr JPH::ObjectLayer MOVING = 1;
	static constexpr JPH::ObjectLayer NUM_LAYERS = 2;    
}

namespace BroadPhaseLayers
{
	static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
	static constexpr JPH::BroadPhaseLayer MOVING(1);
	static constexpr JPH::uint NUM_LAYERS(2);
};

class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
{
public:
	[[nodiscard]] bool ShouldCollide(const JPH::ObjectLayer inObject1, const JPH::ObjectLayer inObject2) const override
	{
		switch (inObject1)
		{
		case PhysicsLayers::NON_MOVING:
			return inObject2 == PhysicsLayers::MOVING; // Non-moving only collides with moving
		case PhysicsLayers::MOVING:
			return true; // Moving collides with everything
		default:
		//JPH_ASSERT(false);
			return false;
		}
	}
};

class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
{
public:
	BPLayerInterfaceImpl()
	{
		// Create a mapping table from object to broad phase layer
		mObjectToBroadPhase[PhysicsLayers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
		mObjectToBroadPhase[PhysicsLayers::MOVING] = BroadPhaseLayers::MOVING;
	}

	[[nodiscard]] JPH::uint GetNumBroadPhaseLayers() const override
	{
		return BroadPhaseLayers::NUM_LAYERS;
	}

	[[nodiscard]] JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
	{
		//JPH_ASSERT(inLayer < PhysicsLayers::NUM_LAYERS);
		return mObjectToBroadPhase[inLayer];
	}

	#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	[[nodiscard]] const char * GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override
	{
		switch (static_cast<JPH::BroadPhaseLayer::Type>(inLayer))
		{
			case static_cast<JPH::BroadPhaseLayer::Type>(BroadPhaseLayers::NON_MOVING):
				return "NON_MOVING";
			case static_cast<JPH::BroadPhaseLayer::Type>(BroadPhaseLayers::MOVING):
				return "MOVING";
			default:												/*	JPH_ASSERT(false);*/
				return "INVALID";
		}
	}
	#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:
	JPH::BroadPhaseLayer					mObjectToBroadPhase[PhysicsLayers::NUM_LAYERS]{};
};

/// Class that determines if an object layer can collide with a broadphase layer
class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
{
public:
	[[nodiscard]] bool ShouldCollide(const JPH::ObjectLayer inLayer1, const JPH::BroadPhaseLayer inLayer2) const override
	{
		switch (inLayer1)
		{
		case PhysicsLayers::NON_MOVING:
			return inLayer2 == BroadPhaseLayers::MOVING;
		case PhysicsLayers::MOVING:
			return true;
		default:
			//JPH_ASSERT(false);
			return false;
		}
	}
};