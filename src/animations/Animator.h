#pragma once

#include "Animation.h"

namespace Octo
{
    class Animator
    {
        public:
            Animator(Animation* anim);
            
            void updateAnimation(float deltaTime);
            void playAnimation(Animation* pAnim);
            void calculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

            std::vector<glm::mat4> getFinalBoneMatrices() { return mFinalBoneMatrices; }
        private:
            std::vector<glm::mat4> mFinalBoneMatrices;
            Animation* mCurrentAnimation;
            float mCurrentTime, mDeltaTime;
    };
}
