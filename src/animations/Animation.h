#pragma once

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "spdlog/spdlog.h"
#include "fastgltf/core.hpp"
#include "fastgltf/types.hpp"
#include "fastgltf/tools.hpp"

#include "../models/Model.h"

#include <string>
#include <vector>
#include <map>
#include <filesystem>

namespace Octo 
{
    struct PositionKey
    {
        float timeStamp;
        glm::vec3 position;
    };

    struct RotationKey
    {
        float timeStamp;
        glm::quat rotation;
    };

    struct ScaleKey
    {
        float timeStamp;
        glm::vec3 scale;
    };


    class Animation
    {
        public:
            Animation(std::string path);

            glm::mat4 getBoneTransform(std::string boneName, int keyFrame);
            float getDuration() { return m_Duration; }

            glm::mat4 calculateBoneInterpolation(std::string boneName, float timeStamp);
        private:
            void fillTimeStamps(fastgltf::Asset& asset, fastgltf::AnimationChannel& channel, fastgltf::Accessor& inputAccessor, std::string boneName);
            void fillKeyFrames(fastgltf::Asset& asset, fastgltf::AnimationChannel& channel, fastgltf::Accessor& outputAccessor, std::string boneName);

            float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animTime);

            int getPositionKeyFrame(std::string boneName, float timeStamp);
            int getRotationKeyFrame(std::string boneName, float timeStamp);
            int getScaleKeyFrame(std::string boneName, float timeStamp);

            glm::mat4 interpolatePosition(std::string boneName, float timeStamp);
            glm::mat4 interpolateRotation(std::string boneName, float timeStamp);
            glm::mat4 interpolateScale(std::string boneName, float timeStamp);

            std::map<std::string, std::vector<PositionKey>> m_PositionKeyFrames;
            std::map<std::string, std::vector<RotationKey>> m_RotationKeyFrames;
            std::map<std::string, std::vector<ScaleKey>> m_ScaleKeyFrames; 

            float m_Duration = 0.0f;
    };
};