#include "Animator.h"

using Octo::Animator;

Animator::Animator(Animation* anim)
{
    mCurrentTime = 0.0f;
    mCurrentAnimation = anim;

    mFinalBoneMatrices.reserve(100);

    for (int i = 0; i < 100; i++)
        mFinalBoneMatrices.push_back(glm::mat4(1.0f));
}
            
void Animator::updateAnimation(float deltaTime)
{
    mDeltaTime = deltaTime;


    if (mCurrentAnimation)
    {
        mCurrentTime += mCurrentAnimation->getTicksPerSecond() * deltaTime;
        mCurrentTime = fmod(mCurrentTime, mCurrentAnimation->getDuration());
        
        calculateBoneTransform(&mCurrentAnimation->getRootNode(), glm::mat4(1.0f));
    }
}

void Animator::playAnimation(Animation* pAnim)
{
    mCurrentAnimation = pAnim;
    mCurrentTime = 0.0f;
}

void Animator::calculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    Bone* bone = mCurrentAnimation->findBone(nodeName);

    if (bone)
    {
        bone->update(mCurrentTime);
        nodeTransform = bone->getLocalTransform();
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    auto boneInfoMap = mCurrentAnimation->getBoneIDMap();

    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int index = boneInfoMap[nodeName].id;
        glm::mat4 offset = boneInfoMap[nodeName].offset;
        mFinalBoneMatrices[index] = globalTransformation * offset;
    }

    for (int i = 0; i < node->childrenCount; i++)
    {
        calculateBoneTransform(&node->children[i], globalTransformation);
    }
}