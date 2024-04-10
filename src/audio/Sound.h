#pragma once

#include <string>

#include <spdlog/spdlog.h>

#include "../../vendor/miniaudio/miniaudio.h"

#include "Audio.h"

namespace Octo
{
    class Sound 
    {
        public:
            Sound(std::string path, bool is3D = false);
            ~Sound();

            void play();
            void stop();

            void setPosition(glm::vec3 pos) { ma_sound_set_position(&m_Sound, pos.x, pos.y, pos.z); }

            ma_sound* getMaSound() { return &m_Sound; }
        private:
            ma_sound m_Sound;
    };
};