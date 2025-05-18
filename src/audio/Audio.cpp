#include "Audio.h"

#include <spdlog/spdlog.h>

using Octo::Audio;

void Audio::init()
{
   /* m_MaResult = ma_decoder_init_file("../../../../assets/music/30s.wav", NULL, &m_MaDecoder);

    if (m_MaResult != MA_SUCCESS)
    {
        spdlog::error("[AUDIO] CAN'T LOAD AUDIO FILE {}", "asd");
        return;
    }

    m_MaDeviceConfig = ma_device_config_init(ma_device_type_playback);

    m_MaDeviceConfig.playback.format = m_MaDecoder.outputFormat;
    m_MaDeviceConfig.playback.channels = m_MaDecoder.outputChannels;
    m_MaDeviceConfig.sampleRate = m_MaDecoder.outputSampleRate;
    m_MaDeviceConfig.dataCallback = dataCallback;
    m_MaDeviceConfig.pUserData = &m_MaDecoder;

    if (ma_device_init(NULL, &m_MaDeviceConfig, &m_MaDevice) != MA_SUCCESS)
    {
        spdlog::error("[AUDIO] CAN'T INIT AUDIO DEVICE");
        return;
    }

    if (ma_device_start(&m_MaDevice) != MA_SUCCESS)
    {
        spdlog::error("[AUDIO] CAN'T START AUDIO DEVICE");
        return;
    }*/


    m_EngineConfig = ma_engine_config_init();

    m_EngineConfig.listenerCount = 2;

    const ma_result result = ma_engine_init(&m_EngineConfig, &m_Engine);

    if (result != MA_SUCCESS)
    {
        spdlog::error("[AUDIO] CAN'T INIT AUDIO ENGINE!");
        return;
    }

    /*if (ma_sound_init_from_file(&m_Engine, "../../../../assets/music/30s.wav", 0, NULL, NULL, &sound) != MA_SUCCESS)
    {
        spdlog::error("[AUDIO] CAN'T INIT AUDIO FILE!");
        return;
    }*/
}

void Audio::update()
{
    if (m_Listener) m_Listener->setData();
}

void Audio::destroy()
{
    ma_engine_uninit(&m_Engine);
}

void Audio::setListener(AudioListener* listener)
{
    m_Listener = listener;

    m_Listener->setListener(&m_Engine, static_cast<unsigned int>(Listeners::Spatial));
}