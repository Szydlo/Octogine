#include "Window.h"

using Octo::Window;

Window::Window(int width, int height, std::string title)
    : m_Width(width), m_Height(height), m_Title(title)
{
    spdlog::info("Creating window");
}

void Window::setResolution(int width, int height) 
{
    m_Width = width;
    m_Height = height;
}