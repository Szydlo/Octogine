#include "Sound.h"

using Octo::Sound;

Sound::Sound(std::string path, bool is3D)
{
    ma_result result = ma_sound_init_from_file(Audio::getMaEngine(), path.c_str(), 0, NULL, NULL, &m_Sound);

    if (result != MA_SUCCESS)
    {
        spdlog::error("[AUDIO] ERROR WHILE LOADING FILE: {}", path);
        return;
    }

    if (is3D)
        ma_sound_set_pinned_listener_index(&m_Sound, (unsigned int)Listeners::Spatial);
    else
        ma_sound_set_pinned_listener_index(&m_Sound, (unsigned int)Listeners::Regular);
}

void Sound::play()
{
    ma_sound_start(&m_Sound);
}

void Sound::stop()
{
    ma_sound_stop(&m_Sound);
}

Sound::~Sound()
{
    
}