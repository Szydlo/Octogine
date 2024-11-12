#pragma once

#include "../models/Model.h"

#include "Animation.h"

namespace Octo
{
    class Animator
    {
        public:
            Animator(Model* model, Animation* animation);

            void prepareModel();
            void update(double deltaTime);
        private:
            void calculateBonesTransform(Skeleton& skeleton, TBone& bone, glm::mat4 parentTransform, int& index);
            std::vector<glm::mat4> m_finalBoneMatrices;
            
            float m_CurrentTime = 0;
            Animation* m_CurrentAnimation;
            Model* m_CurrentModel;
    };
};