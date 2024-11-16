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

void Animator::playAnimation(Animation* animation)
{
    m_CurrentAnimation = animation;
    m_CurrentTime = 0;
}

void Animator::prepareModel()
{
    for (int i = 0; i < m_finalBoneMatrices.size(); i++)
    {
        m_CurrentModel->getShader().bind();
        m_CurrentModel->getShader().setMat4("finalBonesMatrices[" + std::to_string(i) + "]", m_finalBoneMatrices[i]);
    }
}

void Animator::update(double deltaTime, glm::mat4 parentTransform)
{
    if (!m_CurrentAnimation || !m_CurrentModel) { return; }

    //m_CurrentTime += m_CurrentAnimation->getFramesPerSecond() * deltaTime;
    // TODO @ I THINK GETFRAMESPERSECOND IS FUCKED
    m_CurrentTime += deltaTime;
    if (m_CurrentTime > m_CurrentAnimation->getDuration()) { m_CurrentTime = 0; }

    auto& rootBone = m_CurrentModel->getSkeleton().bones[0];
    int index = 0;

    calculateBonesTransform(m_CurrentAnimation, m_CurrentModel->getSkeleton(), rootBone, parentTransform, index);
}

void Animator::updateBlended(double deltaTime, glm::mat4 parentTransform)
{
    if (!m_CurrentAnimation || !m_CurrentModel) { return; }

    float a = 1.0f;
    float b = m_CurrentAnimation->getDuration() / testAnim->getDuration();
    const float animSpeedMultiplierUp = (1.0f - testFactor) * a + b * testFactor; // Lerp

    a = testAnim->getDuration() / m_CurrentAnimation->getDuration();
    b = 1.0f;
    const float animSpeedMultiplierDown = (1.0f - testFactor) * a + b * testFactor; // Lerp

    // Current time of each animation, "scaled" by the above speed multiplier variables
    m_CurrentTime += deltaTime * animSpeedMultiplierUp;
    m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->getDuration());

    m_HelpTime += deltaTime * animSpeedMultiplierDown;
    m_HelpTime = fmod(m_HelpTime, testAnim->getDuration());

    auto& rootBone = m_CurrentModel->getSkeleton().bones[0];
    int index = 0;

    calculateBonesTransform(m_CurrentAnimation, m_CurrentModel->getSkeleton(), rootBone, parentTransform, index);
}

/*
void Animator::calculateBonesTransform(Animation* anim, Skeleton& skeleton, TBone& bone, glm::mat4 parentTransform, int& index)
{
    auto& boneTransform = anim->calculateBoneInterpolation(bone.name, m_CurrentTime);
    glm::mat4 globalTransform = parentTransform * boneTransform;
    glm::mat4 localTransform = globalTransform * bone.inverseBindMatrix;

    m_finalBoneMatrices[index] = localTransform;
    index++;

    for (auto& child : bone.children)
    {
        auto& childBone = skeleton.bones[skeleton.getBoneID(child)];
        calculateBonesTransform(anim, skeleton, childBone, globalTransform, index);
    }
}*/

void Animator::calculateBonesTransform(Animation* anim, Skeleton& skeleton, TBone& bone, glm::mat4 parentTransform, int& index)
{
    float factor = testFactor;

    auto& boneTransform = anim->calculateBoneInterpolation(bone.name, m_CurrentTime);
    auto& boneTransform2 = testAnim->calculateBoneInterpolation(bone.name, m_HelpTime);

    const glm::quat rot0 = glm::quat_cast(boneTransform);
    const glm::quat rot1 = glm::quat_cast(boneTransform2);
    const glm::quat finalRot = glm::slerp(rot0, rot1, factor);
    glm::mat4 blendedMat = glm::mat4_cast(finalRot);
    blendedMat[3] = (1.0f - factor) * boneTransform[3] + boneTransform2[3] * factor;

    glm::mat4 globalTransform = parentTransform * blendedMat;
    glm::mat4 localTransform = globalTransform * bone.inverseBindMatrix;

    m_finalBoneMatrices[index] = localTransform;
    index++;

    for (auto& child : bone.children)
    {
        auto& childBone = skeleton.bones[skeleton.getBoneID(child)];
        calculateBonesTransform(anim, skeleton, childBone, globalTransform, index);
    }
}