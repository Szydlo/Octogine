#pragma once

#include <glm/glm.hpp>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/RayCast.h>

#include <vector>

namespace Octo
{
    struct ResultHit
    {
        unsigned int bodyID;
    };

    class RayCast
    {
        public:
            static bool Cast(glm::vec3 start, glm::vec3 direction, ResultHit& hit);
            static bool CastMultiple(glm::vec3 start, glm::vec3 direction, std::vector<ResultHit>& hits);
    };
}