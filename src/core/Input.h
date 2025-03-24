#pragma once

#include <GLFW/glfw3.h>

namespace Octo
{
    enum class CursorMode : int {
        disabled = GLFW_CURSOR_DISABLED,
        normal = GLFW_CURSOR_NORMAL,
        hidden = GLFW_CURSOR_HIDDEN
    };

    class Input
    {
        public:
            static void setWindow(GLFWwindow* window) { m_Window = window; }
            
            static bool getKeyDown(const int key) { return glfwGetKey(Input::m_Window, key) == GLFW_PRESS; }
            static bool getKeyUp(const int key) { return glfwGetKey(Input::m_Window, key) == GLFW_RELEASE; }
            static bool getKey(const int key) { return glfwGetKey(Input::m_Window, key) == GLFW_PRESS || glfwGetKey(Input::m_Window, key) == GLFW_RELEASE; }
            
            static CursorMode getCursorMode() { return static_cast<CursorMode>(glfwGetInputMode(Input::m_Window, GLFW_CURSOR)); }
            static void setCursorMode(CursorMode mode) { glfwSetInputMode(Input::m_Window, GLFW_CURSOR, static_cast<int>(mode)); }
           
            static float getDirection(int key, int secondKey);
        private:
            inline static GLFWwindow* m_Window;
    };
};