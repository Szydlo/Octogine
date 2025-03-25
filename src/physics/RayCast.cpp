#include "RayCast.h"

#include <Jolt/Physics/Collision/CastResult.h>
#include <Jolt/Physics/Collision/CollisionCollector.h>
#include <Jolt/Physics/Collision/CollisionCollectorImpl.h>

#include "Physics.h"
#include "JoltUtils.h"

using Octo::RayCast;

bool RayCast::Cast(const glm::vec3 start, const glm::vec3 direction, ResultHit& hit)
{
    JPH::RayCast rayCast;
         
    rayCast.mOrigin = JoltUtils::convertGlmVec3(start);
    rayCast.mDirection = JoltUtils::convertGlmVec3(direction);
         
    JPH::AllHitCollisionCollector<JPH::RayCastBodyCollector> collector;
    Physics::getPhysicsSystem()->GetBroadPhaseQuery().CastRay(rayCast, collector);
    const int num_hits = static_cast<int>(collector.mHits.size());
    const JPH::BroadPhaseCastResult* results = collector.mHits.data();

    hit.bodyID = results[0].mBodyID.GetIndex();

    if (num_hits > 0)
        return true;

    return false;
}

bool RayCast::CastMultiple(const glm::vec3 start, const glm::vec3 direction, std::vector<ResultHit>& hits)
{
    JPH::RayCast rayCast;   
         
    rayCast.mOrigin = JoltUtils::convertGlmVec3(start);
    rayCast.mDirection = JoltUtils::convertGlmVec3(direction);
         
    JPH::AllHitCollisionCollector<JPH::RayCastBodyCollector> collector;
    Physics::getPhysicsSystem()->GetBroadPhaseQuery().CastRay(rayCast, collector);
    const int num_hits = static_cast<int>(collector.mHits.size());
    const JPH::BroadPhaseCastResult* results = collector.mHits.data();

    for (int i = 0; i < num_hits; i++)
    {
        ResultHit hit{};
        hit.bodyID = results[i].mBodyID.GetIndex();
        hits.push_back(hit);
    }

    if (num_hits > 0)
        return true;

    return false;
}


