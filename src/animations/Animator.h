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
            Animator(Model* model);

            void playAnimation(Animation* animation);
            void prepareModel();

            void update(double deltaTime, glm::mat4 parentTransform = glm::mat4(1.0));
            void updateBlended(double deltaTime, glm::mat4 parentTransform);

            float testFactor = 0.0f;
            Animation* testAnim;
        private:
            void calculateBonesTransform(Animation* anim, Skeleton& skeleton, TBone& bone, glm::mat4 parentTransform, int& index);

            std::vector<glm::mat4> m_finalBoneMatrices;
            float m_CurrentTime = 0;
            float m_HelpTime = 0;
            Animation* m_CurrentAnimation = nullptr;
            Model* m_CurrentModel;
    };
};