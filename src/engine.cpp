#include "engine.h"

Engine::Engine(int a_width, int a_height, const char* a_windowName)
{
    this->mScreenWidth = a_width;
    this->mScreenHeight = a_height;
    this->mWindowName = a_windowName;
}

int Engine::Initialize()
{
    // Initialize GLFW.
    glfwInit();

    // Tell GLFW that we want to use OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Tell GLFW that we want to use the OpenGL's core profile.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Do this for mac compatability.
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Create Window.
    // Instantiate the window object.
    this->mWindow = glfwCreateWindow(this->mScreenWidth, this->mScreenHeight, this->mWindowName, NULL, NULL);

    // Make sure that the window is created.
    if(mWindow == NULL)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();

        std::cin.get();
        return 0;
    }

    glfwMakeContextCurrent(mWindow);

    // Initialize GLAD.
    // Make sure that glad has been initialized successfully.
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD." << std::endl;
        
        std::cin.get();
        return 0;
    }

    // Set the viewport
    glViewport(0, 0, this->mScreenWidth, this->mScreenHeight);

    // Setup callbacks.
    // Binds the 'framebuffer_size_callback' method to the window resize event.
    glfwSetFramebufferSizeCallback(mWindow, WindowResize);

    this->SetupOpenGlRendering();

    // Start game loop.
    while(!glfwWindowShouldClose(this->mWindow))
    {
        // Calculate the elapsed time between the current and previous frame.
        float m_frameTime = (float)glfwGetTime();
        float m_deltaTime = m_frameTime - this->mLastFrameTime;
        this->mLastFrameTime = m_frameTime;

        glfwPollEvents();
        this->ProcessInput(this->mWindow);

        glClearColor(this->mClearColor.x, this->mClearColor.y, this->mClearColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Application logic
        this->Update(m_deltaTime);
        this->Draw();

        glfwSwapBuffers(this->mWindow);
    }

    this->UninstallParams();

    glfwTerminate();

    return 1;
}

void WindowResize(GLFWwindow* a_window, int a_width, int a_height)
{
    glViewport(0, 0, a_width, a_height);

    // TODO: Do your resize logic here...
}

void Engine::ProcessInput(GLFWwindow* a_window)
{
    // TODO: Process your input here...

    // If the escape key gets pressed, close the window.
    if(glfwGetKey(a_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(a_window, true);
}

void Engine::SetupOpenGlRendering()
{
    // TODO: Setup OpenGL code here...

     mExample = new Example();
     mExample->DrawSetting();
}

void Engine::Update(float a_deltaTime)
{
    // TODO: Update your logic here...
}

void Engine::Draw()
{
    // TODO: Render your stuff here...
    if(mExample) {
        mExample->Draw();
    }
}

void Engine::UninstallParams() 
{
    // TODO: Uninstall your render params here...
    if(mExample) {
        mExample->Uninstall();
    }
}