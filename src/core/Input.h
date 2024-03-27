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
            inline static void setWindow(GLFWwindow* window) { m_Window = window; }
            
            inline static bool getKeyDown(int key) { return glfwGetKey(Input::m_Window, key) == GLFW_PRESS; }
            inline static bool getKeyUp(int key) { return glfwGetKey(Input::m_Window, key) == GLFW_RELEASE; }
            inline static bool getKey(int key) { return glfwGetKey(Input::m_Window, key) == GLFW_PRESS || glfwGetKey(Input::m_Window, key) == GLFW_RELEASE; }
            
            inline static CursorMode getCursorMode() { return (CursorMode)glfwGetInputMode(Input::m_Window, GLFW_CURSOR); }
            inline static void setCursorMode(CursorMode mode) { glfwSetInputMode(Input::m_Window, GLFW_CURSOR, (int)mode); }
           
            static float getDirection(int key, int secondKey);
        private:
            inline static GLFWwindow* m_Window;
    };
};