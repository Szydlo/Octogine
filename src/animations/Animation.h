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
            explicit Animation(const std::string& path, int animation = 0);

            glm::mat4 getBoneTransform(const std::string& boneName, int keyFrame);

            [[nodiscard]] float getDuration() const { return m_Duration; }
            [[nodiscard]] int getFramesPerSecond() const { return m_framesPerSecond; }

            glm::mat4 calculateBoneInterpolation(const std::string& boneName, float timeStamp);
        
            float currentTime = 0.0f;
        private:
            void fillTimeStamps(const fastgltf::Asset& asset, const fastgltf::AnimationChannel& channel, const fastgltf::Accessor& inputAccessor, const std::string &boneName);
            void fillKeyFrames(const fastgltf::Asset& asset, const fastgltf::AnimationChannel& channel, const fastgltf::Accessor& outputAccessor, const std::string &boneName);

            static float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animTime);

            int getPositionKeyFrame(const std::string& boneName, float timeStamp);
            int getRotationKeyFrame(const std::string& boneName, float timeStamp);
            int getScaleKeyFrame(const std::string& boneName, float timeStamp);

            glm::mat4 interpolatePosition(const std::string& boneName, float timeStamp);
            glm::mat4 interpolateRotation(const std::string& boneName, float timeStamp);
            glm::mat4 interpolateScale(const std::string& boneName, float timeStamp);

            std::map<std::string, std::vector<PositionKey>> m_PositionKeyFrames;
            std::map<std::string, std::vector<RotationKey>> m_RotationKeyFrames;
            std::map<std::string, std::vector<ScaleKey>> m_ScaleKeyFrames; 

            float m_Duration = 0.0f;
            int m_framesPerSecond = -1;
    };
};