#include "Window.h"

using Octo::Window;

Window::Window()
{
    spdlog::info("Creating window");
}

void Window::setResolution(int width, int height) 
{
    m_Width = width;
    m_Height = height;
}