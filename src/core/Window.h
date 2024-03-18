#pragma once

#include "spdlog/spdlog.h"

namespace Octo
{
    class Window
    {
        public:
            Window();
            
            void setResolution(int width, int height);

            int getWidth() { return m_Width; }
            int getHeight() { return m_Height; }
        private:
            int m_Width, m_Height;
    };
};