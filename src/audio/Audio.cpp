#include "Audio.h"

using Octo::Audio;

void Audio::init()
{
    m_MaResult = ma_decoder_init_file("../../../assets/music/30s.wav", NULL, &m_MaDecoder);

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
    }
}


void Audio::destroy()
{
    ma_device_uninit(&m_MaDevice);
    ma_decoder_uninit(&m_MaDecoder);
}

void Audio::dataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    
    if (pDecoder == NULL) 
    {
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    (void)pInput;
}