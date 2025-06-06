#include "Window.h"

#include "Events.h"
#include "Input.h"

#include "../graphics/Renderer.h"
#include "../physics/Physics.h"
#include "../audio/Audio.h"

#include "spdlog/spdlog.h"

using Octo::Window;

Window::Window(const int width, const int height, const std::string &title)
    : m_Width(width), m_Height(height), m_Title(title)
{
    spdlog::info("Creating window");

    if (!glfwInit())
    {
        spdlog::critical("CAN'T INIT GLFW!");
        return;
    }

    m_NativeWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

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
        auto* data = static_cast<Window*>(glfwGetWindowUserPointer(window));
        data->setResolution(width, height);
	});

    Input::setWindow(m_NativeWindow);

    glEnable(GL_DEPTH_TEST);  

    { // INIT OPENGL
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; 

        ImGui::StyleColorsDark();
        //ImGui::SetCurrentContext(m_Context);
        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(m_NativeWindow, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
        ImGui_ImplOpenGL3_Init();
    }  

    Renderer::init();
    Physics::init(); 
    Audio::init();
}

void Window::start()
{
    Events::onStart();
    Events::onNodeStart();
    double lastFrame = getTickCount();

    unsigned int frameCount = 0;
    double lastFrameTime = getTickCount();

    Physics::optimizeBroadPhase();

    while(!glfwWindowShouldClose(m_NativeWindow))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        frameCount++;
        const double currentFrame = getTickCount();

        if (currentFrame - lastFrameTime >= 1.0) // one second
        {
            m_CurrentFPS = frameCount;

            frameCount = 0;
            lastFrameTime = currentFrame;
        }

        m_DeltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Physics::update(m_DeltaTime);
        Audio::update();

        Renderer::startPass();
        Events::onUpdate(m_DeltaTime);
        Renderer::endPass();

        {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2(static_cast<float>(m_Width), static_cast<float>(m_Height));

            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }
        }

        glfwSwapBuffers(m_NativeWindow);
        glfwPollEvents();
    } 
}

void Window::closeWindow() const
{
    glfwSetWindowShouldClose(m_NativeWindow, GLFW_TRUE);
}

Window::~Window()
{
    // Renderer::destroy(); @ TODO doesn't work, crashes.
    Physics::destroy();
    Audio::destroy();


    { // DESTORY OPENGL
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    glfwDestroyWindow(m_NativeWindow);
    glfwTerminate();
}

void Window::setResolution(const int width, const int height)
{
    m_Width = width;
    m_Height = height;

    glViewport(0, 0, width, height);
}