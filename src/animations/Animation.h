#pragma once

#include "glm/glm.hpp"

#include "SkeletalModel.h"
#include "Bone.h"

#include <string>
#include <vector>

namespace Octo
{
    struct AssimpNodeData
    {
        glm::mat4 transformation;
        std::string name;
        int childrenCount;
        std::vector<AssimpNodeData> children;
    };
    
    class Animation
    {
        public:
            Animation() = default;
            Animation(std::string animPath, SkeletalModel* model);

            ~Animation() {}

            Bone* findBone(std::string name);
            
            float getTicksPerSecond() { return mTicksPerSecond; }
            float getDuration() { return mDuration; }
            const AssimpNodeData& getRootNode() { return mRootNode; }

            std::map<std::string, SBoneInfo>& getBoneIDMap() { return mBoneInfoMap; }
        private:
            void readMissingBones(const aiAnimation* anim, SkeletalModel& model);
            void readHeirarcyData(AssimpNodeData& dest, const aiNode* src);

            float mDuration;
            int mTicksPerSecond;
            std::vector<Bone> mBones;
            AssimpNodeData mRootNode;
            std::map<std::string, SBoneInfo> mBoneInfoMap;
    };
};
