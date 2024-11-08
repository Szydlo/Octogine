#pragma once

#include "glm/glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "assimp/anim.h"

#include <vector>
#include <string>

namespace Octo
{
    struct KeyPosition
    {
        glm::vec3 position;
        float timeStamp;
    };

    struct KeyRotation
    {
        glm::quat orientation;
        float timeStamp;
    };

    struct KeyScale
    {
        glm::vec3 scale;
        float timeStamp;
    };

    class Bone
    {
        public:
            Bone(std::string name, int id, const aiNodeAnim* channel);

            void update(float animTime);

            int getPositionIndex(float animTime);
            int getRotationIndex(float animTime);
            int getScaleIndex(float animTime);

            glm::mat4 getLocalTransform() { return mLocalTransform; }
            std::string getBoneName() const { return mName; }
            int getBoneID() { return mID; }
        private:
            float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animTime);
            
            glm::mat4 interpolatePosition(float animTime);
            glm::mat4 interpolateRotation(float animTime);
            glm::mat4 interpolateScaling(float animTime);

            std::vector<KeyPosition> mPositions;
            std::vector<KeyRotation> mRotations;
            std::vector<KeyScale> mScales;

            glm::mat4 mLocalTransform;
            std::string mName;
            int mID;

            int mNumPositions, mNumRotations, mNumScalings;
    };
}