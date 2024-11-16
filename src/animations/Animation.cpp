#include "Animation.h"

using Octo::Animation;

Animation::Animation(std::string path, int animation)
{
    fastgltf::Parser parser;
    std::filesystem::path fpath(path);

    auto data = fastgltf::GltfDataBuffer::FromPath(fpath);
    if (data.error() != fastgltf::Error::None) {
        spdlog::error("error");
        return;
    }

    auto asset = parser.loadGltf(data.get(), fpath.parent_path(), fastgltf::Options::None);
    if (auto error = asset.error(); error != fastgltf::Error::None) {
        spdlog::error("error");
        return;
    }

    spdlog::info("loaded animation");

    auto& assets = asset.get();

    if (assets.animations.size() < 1)
    {
        spdlog::error("there is no anim in this file");
        return;
    }

    auto& anim = assets.animations[animation];

    for (auto& channel : anim.channels)
    {
        auto& boneData = assets.nodes[channel.nodeIndex.value()];
        auto& sampler = anim.samplers[channel.samplerIndex];

        auto& inputAccessor = assets.accessors[sampler.inputAccessor]; // time stamps
        auto& outputAccessor = assets.accessors[sampler.outputAccessor]; // keyframes transforms

        std::string boneName(boneData.name);
        fillTimeStamps(assets, channel, inputAccessor, boneName);
        fillKeyFrames(assets, channel, outputAccessor, boneName);
    }
}

void Animation::fillTimeStamps(fastgltf::Asset& asset, fastgltf::AnimationChannel& channel, fastgltf::Accessor& inputAccessor, std::string boneName)
{
    int framesCount = 0;

    fastgltf::iterateAccessor<float>(asset, inputAccessor, 
        [&](float timeStamp)
        {
            framesCount = std::max((int)(inputAccessor.count), framesCount);

            switch (channel.path)
            {
            case fastgltf::AnimationPath::Translation:
                PositionKey pKey;
                pKey.timeStamp = timeStamp;
                m_PositionKeyFrames[boneName].push_back(pKey);

                m_Duration = std::max(m_Duration, timeStamp);

                break;
            case fastgltf::AnimationPath::Rotation:
                RotationKey rKey;
                rKey.timeStamp = timeStamp;
                m_RotationKeyFrames[boneName].push_back(rKey);

                m_Duration = std::max(m_Duration, timeStamp);

                break;
            case fastgltf::AnimationPath::Scale:
                ScaleKey sKey;
                sKey.timeStamp = timeStamp;
                m_ScaleKeyFrames[boneName].push_back(sKey);

                m_Duration = std::max(m_Duration, timeStamp);

                break;
            }
        }
    );

    m_framesPerSecond = framesCount;
}

void Animation::fillKeyFrames(fastgltf::Asset& asset, fastgltf::AnimationChannel& channel, fastgltf::Accessor& outputAccessor, std::string boneName)
{
    switch (channel.path)
        {
            case fastgltf::AnimationPath::Translation:
                fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(asset, outputAccessor,
                    [&](fastgltf::math::fvec3 output, std::size_t idx)
                    {
                        auto& key = m_PositionKeyFrames[boneName][idx];
                        key.position = glm::vec3(output.x(), output.y(), output.z());
                    }
                );
                break;
            case fastgltf::AnimationPath::Rotation:
                fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec4>(asset, outputAccessor,
                    [&](fastgltf::math::fvec4 output, std::size_t idx)
                    {
                        auto& key = m_RotationKeyFrames[boneName][idx];
                        key.rotation = glm::quat(output.w(), output.x(), output.y(), output.z());
                    }
                );
                break;
            case fastgltf::AnimationPath::Scale:
                fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(asset, outputAccessor,
                    [&](fastgltf::math::fvec3 output, std::size_t idx)
                    {
                        auto& key = m_ScaleKeyFrames[boneName][idx];
                        key.scale = glm::vec3(output.x(), output.y(), output.z());
                    }
                );
                break;  
            default:
                //spdlog::error("not handled anim");
                break;
        }
}

glm::mat4 Animation::getBoneTransform(std::string boneName, int keyFrame)
{
    auto& position = m_PositionKeyFrames[boneName][keyFrame].position;
    auto& rotation = m_RotationKeyFrames[boneName][keyFrame].rotation;
    auto& scale = m_ScaleKeyFrames[boneName][keyFrame].scale;

    glm::mat4 pos = glm::translate(glm::mat4(1.0), position);
    glm::mat4 rot = glm::toMat4(glm::normalize(rotation));
    glm::mat4 sc = glm::scale(glm::mat4(1.0), scale);

    return pos * rot * sc;
}


// @ TODO THIS POSITIONKEYFRAME ETC IS SHIT, FIX IT LATER
// @ TODO QUICK FIX FOR NOW: "m_PositionKeyFrames[boneName].size()-1" FIX IT LATER
int Animation::getPositionKeyFrame(std::string boneName, float timeStamp)
{
    for (int i = 0; i < m_PositionKeyFrames[boneName].size()-1; i++)
        if (timeStamp < m_PositionKeyFrames[boneName][i+1].timeStamp) return i; 

    return 0;
}

int Animation::getRotationKeyFrame(std::string boneName, float timeStamp)
{
    for (int i = 0; i < m_RotationKeyFrames[boneName].size()-1; i++)
        if (timeStamp < m_RotationKeyFrames[boneName][i+1].timeStamp) return i;

    return 0;
}

int Animation::getScaleKeyFrame(std::string boneName, float timeStamp)
{
    for (int i = 0; i < m_ScaleKeyFrames[boneName].size()-1; i++)
        if (timeStamp < m_ScaleKeyFrames[boneName][i+1].timeStamp) return i; 

    return 0;
}


float Animation::getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animTime)
{
    float scaleFactor = 0.0f;
    float midwayLength = animTime - lastTimeStamp;
    float framesDiff = nextTimeStamp - lastTimeStamp;

    scaleFactor = midwayLength / framesDiff;

    return scaleFactor;
}

glm::mat4 Animation::interpolatePosition(std::string boneName, float timeStamp)
{
    int key = getPositionKeyFrame(boneName, timeStamp);
    int key2 = key + 1;

    auto& frame = m_PositionKeyFrames[boneName][key];
    auto& frame2 = m_PositionKeyFrames[boneName][key2];

    float scaleFactor = getScaleFactor(frame.timeStamp, frame2.timeStamp, timeStamp);
    glm::vec3 finalPosition = glm::mix(frame.position, frame2.position, scaleFactor);

    return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Animation::interpolateRotation(std::string boneName, float timeStamp)
{
    int key = getRotationKeyFrame(boneName, timeStamp);
    int key2 = key + 1;

    auto& frame = m_RotationKeyFrames[boneName][key];
    auto& frame2 = m_RotationKeyFrames[boneName][key2];

    float scaleFactor = getScaleFactor(frame.timeStamp, frame2.timeStamp, timeStamp);
    glm::quat finalRotation = glm::slerp(frame.rotation, frame2.rotation, scaleFactor);

    return glm::toMat4(glm::normalize(finalRotation));
}

glm::mat4 Animation::interpolateScale(std::string boneName, float timeStamp)
{
    int key = getScaleKeyFrame(boneName, timeStamp);
    int key2 = key + 1;

    auto& frame = m_ScaleKeyFrames[boneName][key];
    auto& frame2 = m_ScaleKeyFrames[boneName][key2];

    float scaleFactor = getScaleFactor(frame.timeStamp, frame2.timeStamp, timeStamp);
    glm::vec3 finalScale = glm::mix(frame.scale, frame2.scale, scaleFactor);

    return glm::scale(glm::mat4(1.0f), finalScale);
}


glm::mat4 Animation::calculateBoneInterpolation(std::string boneName, float timeStamp)
{
    glm::mat4 position = interpolatePosition(boneName, timeStamp);
    glm::mat4 rotation = interpolateRotation(boneName, timeStamp);
    glm::mat4 scale = interpolateScale(boneName, timeStamp);

    return position * rotation * scale;
}