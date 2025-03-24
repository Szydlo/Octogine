#pragma once

#include <Jolt/Jolt.h>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace JoltUtils {
    static glm::vec3 convertJoltVec3(const JPH::Vec3 vec)
    {
        return {vec.GetX(), vec.GetY(), vec.GetZ()};
    }

    static glm::quat convertJoltQuat(const JPH::Quat quat)
    {
        return {quat.GetW(), quat.GetX(), quat.GetY(), quat.GetZ()};
    }

    static JPH::Vec3 convertGlmVec3(const glm::vec3 vec)
    {
        return {vec.x, vec.y, vec.z};
    }

    static JPH::Quat convertGlmQuat(const glm::quat quat)
    {
        return {quat.w, quat.x, quat.y, quat.z};
    }
};