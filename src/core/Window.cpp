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
    
    Physics::init(); 
}

void Window::start()
{
    Events::onStart();
    double currentFrame;
	double lastFrame = getTickCount();

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

        currentFrame = getTickCount();
        m_DeltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Physics::update(m_DeltaTime);

        Renderer::startPass();

        Events::onUpdate(m_DeltaTime);

        Renderer::endPass();

        {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2((float)m_Width, (float)m_Height);

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

void Window::closeWindow()
{
    glfwSetWindowShouldClose(m_NativeWindow, GLFW_TRUE);
}

Window::~Window()
{
    // Renderer::destroy(); @ TODO doesn't work, crashes.
    Physics::destroy();

    { // DESTORY OPENGL
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    glfwDestroyWindow(m_NativeWindow);
    glfwTerminate();
}

void Window::setResolution(int width, int height) 
{
    m_Width = width;
    m_Height = height;

    glViewport(0, 0, width, height);
}