#pragma once

#include <Jolt/Jolt.h>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace JoltUtils {
    static glm::vec3 convertJoltVec3(JPH::Vec3 vec)
    {
        return {vec.GetX(), vec.GetY(), vec.GetZ()};
    }

    static glm::quat convertJoltQuat(JPH::Quat quat)
    {
        return {quat.GetW(), quat.GetX(), quat.GetY(), quat.GetZ()};
    }

    static JPH::Vec3 convertGlmVec3(glm::vec3 vec)
    {
        return JPH::Vec3(vec.x, vec.y, vec.z);
    }

    static JPH::Quat convertGlmQuat(glm::quat quat)
    {
        return JPH::Quat(quat.w, quat.x, quat.y, quat.z);
    }
};