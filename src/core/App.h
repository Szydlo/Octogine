#pragma once

#include <memory>
#include "spdlog/spdlog.h"

#include "window.h"

namespace Octo 
{
    class App 
    {
        public:
            App();
            ~App();
        private:
            std::unique_ptr<Window> m_Window;
    };
};