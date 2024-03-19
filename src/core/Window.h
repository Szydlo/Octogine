#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "spdlog/spdlog.h"

#include <string>

#include "Events.h"
#include "Input.h"

namespace Octo
{
    class Window
    {
        public:
            Window(int width = 1280, int height = 720, std::string title = "Octogine application");
            ~Window();
            
            void start();
            void setResolution(int width, int height);
            void closeWindow();
            bool shouldWindowClose() { return glfwWindowShouldClose(m_NativeWindow); }

            int getWidth() { return m_Width; }
            int getHeight() { return m_Height; }

            double getTickCount() { return glfwGetTime(); }
        private:
            int m_Width, m_Height;
            double m_DeltaTime;
            std::string m_Title;
            GLFWwindow* m_NativeWindow;
    };
};