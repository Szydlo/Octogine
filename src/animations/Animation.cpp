#include "Animation.h"

using Octo::Animation;

Animation::Animation(std::string animPath, SkeletalModel* model)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(animPath, aiProcess_Triangulate);
    assert(scene && scene->mRootNode);
    
    auto animation = scene->mAnimations[1];

    mDuration = animation->mDuration;
    mTicksPerSecond = animation->mTicksPerSecond;

    readHeirarcyData(mRootNode, scene->mRootNode);
    readMissingBones(animation, *model);
}

Octo::Bone* Animation::findBone(std::string name)
{
    auto iter = std::find_if(mBones.begin(), mBones.end(), 
        [&](const Octo::Bone& bone)
        {
            return bone.getBoneName() == name;
        }
    );

    if (iter == mBones.end()) return nullptr;
    else return &(*iter);
}

void Animation::readMissingBones(const aiAnimation* anim, SkeletalModel& model)
{
    int size = anim->mNumChannels;

    auto& boneInfoMap = model.getBoneInfoMap();
    int& boneCount = model.getBoneCount();

    for (int i = 0; i < size; i++)
    {
        auto channel = anim->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }

        mBones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel));
    }

    mBoneInfoMap = boneInfoMap;
}

void Animation::readHeirarcyData(AssimpNodeData& dest, const aiNode* src)
{
    assert(src);

    dest.name = src->mName.data;
    dest.transformation = sConvertMatrixToGLMFormat(src->mTransformation);
    dest.childrenCount = src->mNumChildren;

    for (int i = 0; i < src->mNumChildren; i++)
    {
        AssimpNodeData newData;
        readHeirarcyData(newData, src->mChildren[i]);
        dest.children.push_back(newData);
    }
}