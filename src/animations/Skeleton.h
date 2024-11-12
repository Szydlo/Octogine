#pragma once

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <vector>
#include <string>

namespace Octo
{
    struct TBone
    {
        std::vector<std::string> children;
        std::string name;
        glm::mat4 inverseBindMatrix;
    };

    struct Skeleton
    {
        std::string name;
        std::vector<TBone> bones;

        int getBoneID(std::string name)
        {
            int i = 0;

            for (auto& bone : bones)
            {
                if (bone.name == name) return i;
                i++;
            }

            return -1;
        } 
    };
}