#pragma once

#include "spdlog/spdlog.h"
#include <string>

namespace Octo
{
    class Window
    {
        public:
            Window(int width = 1280, int height = 720, std::string title = "Octogine application");
            
            void setResolution(int width, int height);

            int getWidth() { return m_Width; }
            int getHeight() { return m_Height; }
        private:
            int m_Width, m_Height;
            std::string m_Title;
    };
};