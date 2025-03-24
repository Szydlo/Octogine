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
            explicit Window(int width = 1280, int height = 720, const std::string &title = "Octogine application");
            ~Window();
            
            void start();
            void setResolution(int width, int height);
            void closeWindow() const;

            [[nodiscard]] bool shouldWindowClose() const { return glfwWindowShouldClose(m_NativeWindow); }
            [[nodiscard]] int getWidth() const { return m_Width; }
            [[nodiscard]] int getHeight() const { return m_Height; }

            static double getTickCount() { return glfwGetTime(); }

            [[nodiscard]] double getDeltaTime() const { return m_DeltaTime; }
            [[nodiscard]] unsigned int getFPS() const { return m_CurrentFPS; }
        private:
            int m_Width, m_Height;
            double m_DeltaTime;
            std::string m_Title;
            GLFWwindow* m_NativeWindow;

            unsigned int m_CurrentFPS = 0;
    };
};