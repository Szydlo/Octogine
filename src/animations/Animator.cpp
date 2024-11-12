#include "Animator.h"

using Octo::Animator;

Animator::Animator(Model* model, Animation* animation)
    : m_CurrentModel(model), m_CurrentAnimation(animation)
{
    m_finalBoneMatrices.resize(100);

    for (auto& matrix : m_finalBoneMatrices)
    {
        matrix = glm::mat4(1.0);
    }
}

void Animator::prepareModel()
{
    for (int i = 0; i < m_finalBoneMatrices.size(); i++)
    {
        m_CurrentModel->getShader().bind();
        m_CurrentModel->getShader().setMat4("finalBonesMatrices[" + std::to_string(i) + "]", m_finalBoneMatrices[i]);
    }
}

void Animator::update(double deltaTime)
{
    m_CurrentTime += deltaTime;
    if (m_CurrentTime > m_CurrentAnimation->getDuration()) { m_CurrentTime = 0; }

    //spdlog::info(m_CurrentTime);

    auto& rootBone = m_CurrentModel->getSkeleton().bones[0];
    int index = 0;
    calculateBonesTransform(m_CurrentModel->getSkeleton(), rootBone, glm::mat4(1.0), index);
}

void Animator::calculateBonesTransform(Skeleton& skeleton, TBone& bone, glm::mat4 parentTransform, int& index)
{
    auto& boneTransform = m_CurrentAnimation->calculateBoneInterpolation(bone.name, m_CurrentTime);
    glm::mat4 globalTransform = parentTransform * boneTransform;
    glm::mat4 localTransform = globalTransform * bone.inverseBindMatrix;

    m_finalBoneMatrices[index] = localTransform;
    index++;

    for (auto& child : bone.children)
    {
        auto& childBone = skeleton.bones[skeleton.getBoneID(child)];
        calculateBonesTransform(skeleton, childBone, globalTransform, index);
    }
}