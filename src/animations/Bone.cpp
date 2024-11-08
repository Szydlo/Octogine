#include "Bone.h"

using Octo::Bone;

Bone::Bone(std::string name, int id, const aiNodeAnim* channel)
    : mName(name), mID(id), mLocalTransform(1.0f)
{
    mNumPositions = channel->mNumPositionKeys;
    mNumRotations = channel->mNumRotationKeys;
    mNumScalings = channel->mNumScalingKeys;

    for (int posIndex = 0; posIndex <= channel->mNumPositionKeys; posIndex++)
    {
        aiVector3D aiPos = channel->mPositionKeys[posIndex].mValue;
        KeyPosition keyPos;

        keyPos.position = glm::vec3(aiPos.x, aiPos.y, aiPos.z);
        keyPos.timeStamp = channel->mPositionKeys[posIndex].mTime;

        mPositions.push_back(keyPos);
    }

    for (int rotIndex = 0; rotIndex <= channel->mNumRotationKeys; rotIndex++)
    {
        aiQuaternion aiOrientation = channel->mRotationKeys[rotIndex].mValue;
        KeyRotation keyRot;

        keyRot.orientation = glm::quat(aiOrientation.w, aiOrientation.x, aiOrientation.y, aiOrientation.z);
        keyRot.timeStamp = channel->mRotationKeys[rotIndex].mTime;

        mRotations.push_back(keyRot);
    }

    for (int scaleIndex = 0; scaleIndex <= channel->mNumScalingKeys; scaleIndex++)
    {
        aiVector3D scale = channel->mScalingKeys[scaleIndex].mValue;
        KeyScale keyScale;

        keyScale.scale = glm::vec3(scale.x, scale.y, scale.z);
        keyScale.timeStamp = channel->mScalingKeys[scaleIndex].mTime;

        mScales.push_back(keyScale);
    }
}

void Bone::update(float animTime)
{
    glm::mat4 translation = interpolatePosition(animTime);
    glm::mat4 rotation = interpolateRotation(animTime);
    glm::mat4 scale = interpolateScaling(animTime);

    mLocalTransform = translation * rotation * scale;
}

int Bone::getPositionIndex(float animTime)
{
    for (int i = 0; i < mNumPositions - 1; i++)
    {
        if (animTime < mPositions[i + 1].timeStamp)
            return i;
    }

    assert(0);
}

int Bone::getRotationIndex(float animTime)
{
    for (int i = 0; i < mNumRotations - 1; i++)
    {
        if (animTime < mRotations[i + 1].timeStamp)
            return i;
    }

    assert(0);
}

int Bone::getScaleIndex(float animTime)
{
    for (int i = 0; i < mNumScalings - 1; i++)
    {
        if (animTime < mScales[i + 1].timeStamp)
            return i;
    }

    assert(0);
}

float Bone::getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animTime)
{
    float scaleFactor = 0.0f;
    float midwayLength = animTime - lastTimeStamp;
    float framesDiff = nextTimeStamp - lastTimeStamp;

    scaleFactor = midwayLength / framesDiff;

    return scaleFactor;
}

glm::mat4 Bone::interpolatePosition(float animTime)
{
    if (mNumPositions == 1)
        return glm::translate(glm::mat4(1.0f), mPositions[0].position);

    int pIndex0 = getPositionIndex(animTime);
    int pIndex = pIndex0 + 1;

    float scaleFactor = getScaleFactor(mPositions[pIndex0].timeStamp, mPositions[pIndex].timeStamp, animTime);
    glm::vec3 finalPosition = glm::mix(mPositions[pIndex0].position, mPositions[pIndex].position, scaleFactor);
    
    return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::interpolateRotation(float animTime)
{
    if (mNumRotations == 1)
    {
        auto rot = glm::normalize(mRotations[0].orientation);
        return glm::toMat4(rot);
    }

    int pIndex0 = getRotationIndex(animTime);
    int pIndex = pIndex0 + 1;

    float scaleFactor = getScaleFactor(mRotations[pIndex0].timeStamp, mRotations[pIndex].timeStamp, animTime);
    glm::quat finalRotation = glm::slerp(mRotations[pIndex0].orientation, mRotations[pIndex].orientation, scaleFactor);
    
    finalRotation = glm::normalize(finalRotation);

    return glm::toMat4(finalRotation);
}


glm::mat4 Bone::interpolateScaling(float animTime)
{
    if (mNumScalings == 1)
        return glm::scale(glm::mat4(1.0f), mScales[0].scale);

    int pIndex0 = getScaleIndex(animTime);
    int pIndex = pIndex0 + 1;

    float scaleFactor = getScaleFactor(mScales[pIndex0].timeStamp, mScales[pIndex].timeStamp, animTime);
    glm::vec3 finalScale = glm::mix(mScales[pIndex0].scale, mScales[pIndex].scale, scaleFactor);
    
    return glm::scale(glm::mat4(1.0f), finalScale);
}