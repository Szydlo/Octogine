#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "spdlog/spdlog.h"

#include <string>

#include "Events.h"
#include "Input.h"

#include "../graphics/Renderer.h"
#include "../physics/Physics.h"
#include "../audio/Audio.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

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
            double getDeltaTime() { return m_DeltaTime; }

            unsigned int getFPS() { return m_CurrentFPS; }
        private:
            int m_Width, m_Height;
            double m_DeltaTime;
            std::string m_Title;
            GLFWwindow* m_NativeWindow;

            unsigned int m_CurrentFPS = 0;
    };
};