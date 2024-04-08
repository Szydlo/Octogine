#pragma once

#include <spdlog/spdlog.h>

#include "../../vendor/miniaudio/miniaudio.h"

namespace Octo
{
    class Audio
    {
        public:
            static void init();
            static void destroy();
            static void dataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
        private:
            inline static ma_result m_MaResult;
            inline static ma_decoder m_MaDecoder;
            inline static ma_device_config m_MaDeviceConfig;
            inline static ma_device m_MaDevice;
    };
}