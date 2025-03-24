#pragma once

#include "../../vendor/miniaudio/miniaudio.h"

#include "AudioListener.h"

namespace Octo
{
    enum class Listeners : unsigned int
    {
        Regular, Spatial
    };

    class Audio
    {
        public:
            static void init();
            static void destroy();
            static void update();

            static ma_engine* getMaEngine() { return &m_Engine; }

            static void setVolume(const float volume) { ma_engine_set_volume(&m_Engine, volume); }
            static float getVolume() { return ma_engine_get_volume(&m_Engine); }

            static void setListener(AudioListener* listener);
        private:
            inline static ma_engine m_Engine;
            inline static ma_engine_config m_EngineConfig;
            inline static ma_sound sound;
            inline static AudioListener* m_Listener = nullptr;
    };
}