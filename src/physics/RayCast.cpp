#include "RayCast.h"

using Octo::RayCast;

bool RayCast::Cast(glm::vec3 start, glm::vec3 direction, ResultHit& hit)
{
    JPH::RayCast raycast;
         
    raycast.mOrigin = JoltUtils::convertGlmVec3(start);
    raycast.mDirection = JoltUtils::convertGlmVec3(direction);
         
    JPH::AllHitCollisionCollector<JPH::RayCastBodyCollector> collector;
    Physics::getPhysicsSystem()->GetBroadPhaseQuery().CastRay(raycast, collector);
    int num_hits = (int)collector.mHits.size();
    JPH::BroadPhaseCastResult *results = collector.mHits.data();

    hit.bodyID = results[0].mBodyID.GetIndex();

    results[0];

    if (num_hits > 0)
        return true;

    return false;
}

bool RayCast::CastMultiple(glm::vec3 start, glm::vec3 direction, std::vector<ResultHit>& hits)
{
    JPH::RayCast raycast;   
         
    raycast.mOrigin = JoltUtils::convertGlmVec3(start);
    raycast.mDirection = JoltUtils::convertGlmVec3(direction);
         
    JPH::AllHitCollisionCollector<JPH::RayCastBodyCollector> collector;
    Physics::getPhysicsSystem()->GetBroadPhaseQuery().CastRay(raycast, collector);
    int num_hits = (int)collector.mHits.size();
    JPH::BroadPhaseCastResult *results = collector.mHits.data();

    for (int i = 0; i < num_hits; i++)
    {
        ResultHit hit;
        hit.bodyID = results[i].mBodyID.GetIndex();;
        hits.push_back(hit);
    }

    if (num_hits > 0)
        return true;

    return false;
}


