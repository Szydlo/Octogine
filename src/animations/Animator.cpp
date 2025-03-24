#include "Animator.h"

using Octo::Animator;

Animator::Animator(Model* model)
    : m_CurrentModel(model)
{
    m_finalBoneMatrices.resize(100);

    for (auto& matrix : m_finalBoneMatrices)
    {
        matrix = glm::mat4(1.0);
    }
}

void Animator::prepareModel() const
{
    for (int i = 0; i < m_finalBoneMatrices.size(); i++)
    {
        m_CurrentModel->getShader().bind();
        m_CurrentModel->getShader().setMat4("finalBonesMatrices[" + std::to_string(i) + "]", m_finalBoneMatrices[i]);
    }
}

void Animator::updateAnimation(Animation* anim, const double deltaTime, const glm::mat4 parentTransform)
{
    if (!anim || !m_CurrentModel) { return; }

    //m_CurrentTime += m_CurrentAnimation->getFramesPerSecond() * deltaTime;
    // TODO @ I THINK GETFRAMESPERSECOND IS FUCKED
    anim->currentTime += deltaTime;
    if (anim->currentTime > anim->getDuration()) { anim->currentTime = 0; }

    auto& rootBone = m_CurrentModel->getSkeleton().bones[0];
    int index = 0;

    calculateBonesTransform(anim, m_CurrentModel->getSkeleton(), rootBone, parentTransform, index);
}

void Animator::updateBlended(Animation* baseAnim, Animation* secondAnim, const float blendFactor, const double deltaTime, const glm::mat4& parentTransform)
{
    if (!baseAnim || !m_CurrentModel) { return; }

    // @ TODO THIS TIME CALC MIGHT BE A LITTLE BIT BUGGED

    float a = 1.0f;
    float b = baseAnim->getDuration() / secondAnim->getDuration();
    const float animSpeedMultiplierUp = (1.0f - blendFactor) * a + b * blendFactor; // Lerp

    a = secondAnim->getDuration() / baseAnim->getDuration();
    b = 1.0f;
    const float animSpeedMultiplierDown = (1.0f - blendFactor) * a + b * blendFactor; // Lerp

    // Current time of each animation, "scaled" by the above speed multiplier variables
    baseAnim->currentTime += deltaTime * animSpeedMultiplierUp;
    baseAnim->currentTime = fmod(baseAnim->currentTime, baseAnim->getDuration());

    secondAnim->currentTime += deltaTime * animSpeedMultiplierDown;
    secondAnim->currentTime = fmod(secondAnim->currentTime, secondAnim->getDuration());

    auto& rootBone = m_CurrentModel->getSkeleton().bones[0];
    int index = 0;

    calculateBonesTransformsBlended(baseAnim, secondAnim, blendFactor, m_CurrentModel->getSkeleton(), rootBone, parentTransform, index);
}

void Animator::calculateBonesTransform(Animation* anim, Skeleton& skeleton, TBone& bone, const glm::mat4 parentTransform, int& index)
{
    const auto boneTransform = anim->calculateBoneInterpolation(bone.name, anim->currentTime);
    const glm::mat4 globalTransform = parentTransform * boneTransform;
    const glm::mat4 localTransform = globalTransform * bone.inverseBindMatrix;

    m_finalBoneMatrices[index] = localTransform;
    index++;

    for (auto& child : bone.children)
    {
        auto& childBone = skeleton.bones[skeleton.getBoneID(child)];
        calculateBonesTransform(anim, skeleton, childBone, globalTransform, index);
    }
}

void Animator::calculateBonesTransformsBlended(Animation* baseAnim, Animation* secondAnim, const float blendFactor, Skeleton& skeleton, TBone& bone, const glm::mat4 parentTransform, int& index)
{
    auto boneTransform = baseAnim->calculateBoneInterpolation(bone.name, baseAnim->currentTime);
    auto boneTransform2 = secondAnim->calculateBoneInterpolation(bone.name, secondAnim->currentTime);

    const glm::quat rot0 = glm::quat_cast(boneTransform);
    const glm::quat rot1 = glm::quat_cast(boneTransform2);
    const glm::quat finalRot = glm::slerp(rot0, rot1, blendFactor);
    glm::mat4 blendedMat = glm::mat4_cast(finalRot);
    blendedMat[3] = (1.0f - blendFactor) * boneTransform[3] + boneTransform2[3] * blendFactor;

    const glm::mat4 globalTransform = parentTransform * blendedMat;
    const glm::mat4 localTransform = globalTransform * bone.inverseBindMatrix;

    m_finalBoneMatrices[index] = localTransform;
    index++;

    for (auto& child : bone.children)
    {
        auto& childBone = skeleton.bones[skeleton.getBoneID(child)];
        calculateBonesTransformsBlended(baseAnim, secondAnim, blendFactor, skeleton, childBone, globalTransform, index);
    }
}