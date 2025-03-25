#pragma once

#include "../models/Model.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "Animation.h"

namespace Octo
{
    class Animator
    {
        public:
            explicit Animator(Model* model);

            void prepareModel() const;

            void updateAnimation(Animation* anim, double deltaTime, const glm::mat4 &parentTransform = glm::mat4(1.0));
            void updateBlended(Animation* baseAnim, Animation* secondAnim, float blendFactor, double deltaTime, const glm::mat4 &parentTransform);
        private:
            void calculateBonesTransform(Animation* anim, Skeleton& skeleton, TBone& bone, const glm::mat4 &parentTransform, int& index);
            void calculateBonesTransformsBlended(Animation* baseAnim, Animation* secondAnim, float blendFactor, Skeleton& skeleton, TBone& bone, const glm::mat4 &parentTransform, int& index);

            std::vector<glm::mat4> m_finalBoneMatrices;
            Model* m_CurrentModel;
    };
};