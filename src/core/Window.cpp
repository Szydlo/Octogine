#include "Window.h"

using Octo::Window;

Window::Window(int width, int height, std::string title)
    : m_Width(width), m_Height(height), m_Title(title)
{
    spdlog::info("Creating window");

    if (!glfwInit())
    {
        spdlog::critical("CAN'T INIT GLFW!");
        return;
    }

    m_NativeWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if (!m_NativeWindow)
    {
        spdlog::critical("CAN'T INIT GLFW NATIVE WINDOW!");
        return;
    }

    glfwMakeContextCurrent(m_NativeWindow);
    glfwSetWindowUserPointer(m_NativeWindow, reinterpret_cast<void*>(this));

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::critical("CAN'T INIT GLAD");
        return;
    }

    spdlog::info("Successfully created window, starting main loop");

	glfwSetKeyCallback(m_NativeWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{	
        if (action == GLFW_PRESS) Events::onClick(key, true);
        if (action == GLFW_RELEASE) Events::onClick(key, false);
	});

    
	glfwSetCursorPosCallback(m_NativeWindow, [](GLFWwindow* window, double x, double y)
	{
		Events::onMouseMove(x, y);
	});

	glfwSetFramebufferSizeCallback(m_NativeWindow, [](GLFWwindow* window, int width, int height)
	{
        Window* data = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        data->setResolution(width, height);
	});

    Input::setWindow(m_NativeWindow);

    glEnable(GL_DEPTH_TEST);  
}

void Window::start()
{
    Events::onStart();
    double currentFrame;
	double lastFrame = getTickCount();

    while(!glfwWindowShouldClose(m_NativeWindow))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        currentFrame = getTickCount();
        m_DeltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Renderer::startPass();

        Events::onUpdate(m_DeltaTime);

        Renderer::endPass();

        glfwSwapBuffers(m_NativeWindow);
        glfwPollEvents();
    } 
}

void Window::closeWindow()
{
    glfwSetWindowShouldClose(m_NativeWindow, GLFW_TRUE);
}

Window::~Window()
{
    // Renderer::destroy(); @ TODO doesn't work, crashes.

    glfwDestroyWindow(m_NativeWindow);
    glfwTerminate();
}

void Window::setResolution(int width, int height) 
{
    m_Width = width;
    m_Height = height;

    glViewport(0, 0, width, height);
}